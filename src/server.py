#!/usr/bin/env python

# Do necessary imports
import socket
import thread
import time
import sys
import re
from subprocess import Popen, PIPE
 
# Setup socket for global usage
HOST = ""
PORT = 8888
if len(sys.argv) > 1:
    PORT = int(sys.argv[1])
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.setblocking(False)


# Save information for each game played
game_states = {}     # dictionary for game info
class state_o:
    def __init__(self):
        self.side = "0"
        self.mode = "0"
        self.diff = "0"
        self.ip = ""
        self.port = 0
        self.diff2 = "0"
        self.started = False
    def __repr__(self):
        if self.port > 0:
            return "%s, %s, %s, %s, %s" % (self.side, self.mode, self.diff, self.ip, self.port)
        return "%s, %s, %s" % (self.side, self.mode, self.diff)
    def __str__(self):
        if self.port > 0:
            return "%s, %s, %s, %s, %s" % (self.side, self.mode, self.diff, self.ip, self.port)
        return "%s, %s, %s" % (self.side, self.mode, self.diff)
    def start(self):
        self.proc = Popen('board', shell=False, stdout=PIPE, stdin=PIPE, stderr=PIPE)
        self.started = True
        #self.send(self.diff)
    def end(self):
        if self.started:
            self.send("@")
            self.proc.kill()
    def comm(self, info=""):
        return self.proc.communicate(info)[0]
    def send(self,info):
        if self.started:
            self.proc.stdin.write("%s\n" % info)
    def flush(self):
        print "flushing"
        self.proc.communicate()
        print "dont"
        return
        firstempty = False
        while True:
            f = self.proc.stdout.readline()
            print "hi\n"
            if len(f) <= 1:
                if firstempty:
                    return
                firstempty = True
            else:
                firstempty = False
    def read(self):
        if self.started:
            return self.proc.stdout.readline()[:-1]
        return ""


#############################
# CALLBACKS FOR SERVER GAME #
#############################

def setside(tag, side):
    sys.stdout.write("Game %d set to %s\n" % (tag, side))
    game_states[tag].side = side

def setmode(tag, mode):
    sys.stdout.write("Game %d set to %s\n" % (tag, mode))
    game_states[tag].mode = mode
    if "AI-AI" in mode:
        ip_addr = re.findall(ip_r,mode)[0][0]
        port = int(re.findall(port_r,mode)[0][0])
        diff = re.findall(difficulty_r,mode)[0][0]
        diff2 = re.findall(difficulty_r,mode)[1][0]
        game_states[tag].mode = "AI-AI"
        game_states[tag].ip = ip_addr
        game_states[tag].port = port
        game_states[tag].diff = diff
        game_states[tag].diff2 = diff2

def setdifficulty(tag, level):
    sys.stdout.write("Game %d set to %s\n" % (tag, level))
    game_states[tag].diff = level[0]

def doexit(tag):
    game_states[tag].end()
    pass

def dodisplay(tag,conn):
    game_states[tag].send("d")
    sys.stdout.write("Display game %d\n" % tag)
    for i in range(10):
        info = "%s\n" % game_states[tag].read()
        conn.send(info)
        sys.stdout.write(info)

def doundo(tag):
    game_states[tag].send("u")
    resp = game_states[tag].read()
    return "G" in resp

def domove(tag, move):
    game_states[tag].send(move)
    sys.stdout.write("Game %d: Sending move %s\n" % (tag,move))
    resp = game_states[tag].read()
    if "I" in resp:
        sys.stdout.write("Game %d: Bad move\n" % tag)
        return False
    sys.stdout.write("%s\n"%resp)
    return True


#######################
# Command Information #
#######################

# Format regex
ip_r = r'((([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]))'
port_r = r'(([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))'
difficulty_r = r'\s*((EASY)|(MEDIUM)|(HARD))\s*'

# Command regex
display_r = r'^\s*DISPLAY\s*'
exit_r = r'^\s*EXIT\s*'
undo_r = r'^\s*UNDO\s*'
move_r = r'^\s*[A-H]\s*[0-8]\s*$'
comment_r = r'^\s*;.*'

# Settings regex
sides =     [
        r'\s*WHITE\s*',
        r'\s*BLACK\s*'
        ]

difficulties =     [
        r'\s*EASY\s*',
        r'\s*MEDIUM\s*',
        r'\s*HARD\s*'
        ]

gamemodes =    [
        r"\s*HUMAN-AI\s*",
        r"\s*AI-AI\s+%s\s+%s\s+%s\s+%s" % (ip_r, port_r, difficulty_r, difficulty_r)
        ]

# State machine constants
tag_count = 0         # game info counter

# Perform a check for correct command structure
def check(tag, msg, container, funct):
    for item in container:
        if re.match("^%s$" % item,msg):
            funct(tag,msg)
            return True
    return False

# Run the game for an individual connection as a thread
def run(conn):
    def game():

        # Assign a tag and start associated thread
        global tag_count
        tag = tag_count
        tag_count += 1
        conn.send("WELCOME\n")
        conn.setblocking(True)

        hasSide = True
        hasMode = False
        hasDiff = False
        started = False

        sys.stdout.write("Creating game %d\n" % tag)
        global game_states
        game_states[tag] = state_o()
        while True:
            
            # Receive a message and convert to uppercase
            try:
                msg = conn.recv(1024).strip()
                msg = msg.upper()

                # Check for comments
                if re.match(comment_r,msg):
                    continue

                # EXIT
                elif re.match(exit_r,msg):
                    conn.send("OK\n")
                    doexit(tag)
                    conn.close()
                    sys.stdout.write("Game %d ended\n" % tag)
                    return 0

                # DISPLAY
                elif re.match(display_r,msg):
                    conn.send("OK\n")
                    dodisplay(tag,conn)
                    continue

                # UNDO
                elif re.match(undo_r,msg):
                    conn.send("OK\n")
                    if not doundo(tag):
                        conn.send(";Nothing to undo\n")
                    continue

                # EXECUTE A MOVE
                elif started:
                    if (re.match(move_r,msg) and domove(tag,msg)):
                        conn.send("OK\n")
                    else:
                        conn.send("ILLEGAL\n;Bad move\n")
                        continue

                # CHOOSE MODE: HUMAN-AI, AI-AI
                elif check(tag,msg,gamemodes,setmode):
                    conn.send("OK\n")
                    hasMode = True

                # CHOOSE SIDE: BLACK, WHITE
                #elif check(tag,msg,sides,setside):
                #    conn.send("OK\n")
                #    hasSide = True

                # CHOOSE DIFFICUTLY: EASY, MEDIUM, HARD
                elif check(tag,msg,difficulties,setdifficulty):
                    conn.send("OK\n")
                    hasDiff = True

                # Unknown command given or settings not satisfied
                else:
                    conn.send("ILLEGAL\n;Please enter all settings\n")

                # START GAME
                if (not started and hasMode and hasSide and hasDiff):
                    started = True
                    sys.stdout.write("Game %d started\n" % tag)
                    conn.send(";Game started\n")
                    game_states[tag].start()

            except socket.error:
                sys.stdout.write("Game %d ended\n" % tag)
                return 0
            
    # Start game thread
    thread.start_new_thread(game, ())

# Setup socket and listen for connections
def main():
    server.bind((HOST,PORT))
    server.listen(1)
    while True:
        try:
            while True:
                try:
                    conn,addr = server.accept()
                except socket.error:
                    break
                run(conn)
            time.sleep(.1)
        except (SystemExit, KeyboardInterrupt):
            server.close()
            break

if __name__ == '__main__':
    main()
