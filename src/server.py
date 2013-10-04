#!/usr/bin/env python

# Do necessary imports
import socket
import thread
import time
import sys
import re
 
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
    def __repr__(self):
        if self.port > 0:
            return "%s, %s, %s, %s, %s" % (self.side, self.mode, self.diff, self.ip, self.port)
        return "%s, %s, %s" % (self.side, self.mode, self.diff)
    def __str__(self):
        if self.port > 0:
            return "%s, %s, %s, %s, %s" % (self.side, self.mode, self.diff, self.ip, self.port)
        return "%s, %s, %s" % (self.side, self.mode, self.diff)


#############################
# CALLBACKS FOR SERVER GAME #
#############################

def setside(tag, side):
    sys.stdout.write("Game %d set to %s\n" % (tag, side))
    game_states[tag].side = side

def setmode(tag, mode):
    sys.stdout.write("Game %d set to %s\n" % (tag, mode))
    if "AI-AI" in mode:
        ip_addr = re.findall(ip_r,mode)[0][0]
        port = int(re.findall(port_r,mode)[0][0])
        game_states[tag].ip = ip_addr
        game_states[tag].port = port
    game_states[tag].mode = mode

def setdifficulty(tag, level):
    sys.stdout.write("Game %d set to %s\n" % (tag, level))
    game_states[tag].diff = level

def doexit(tag):
    pass

def dodisplay(tag):
    pass

def doundo(tag):
    pass

def domove(tag, move):
    return True


#######################
# Command Information #
#######################

ip_r = r'((([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]))'
port_r = r'(([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))'

sides =     [
        "WHITE",
        "BLACK"
        ]

commands =     [
        "EXIT",
        "DISPLAY",
        "UNDO",
        ]

difficulties =     [
        "EASY",
        "MEDIUM",
        "HARD"
        ]

gamemodes =    [
        r"HUMAN-AI",
        r"AI-AI %s %s" % (ip_r, port_r)
        ]

display_r = r'^\s*DISPLAY\s*'
exit_r = r'^\s*EXIT\s*'
undo_r = r'^\s*UNDO\s*'
move_r = r'^\s*[A-H]\s*[0-8]\s*$'
comment_r = r'^\s*;.*'


# State machine constants
CHOOSESIDE=0
CHOOSEMODE=1
CHOOSEDIFFICULTY=2
GAMEPLAY=3
tag_count = 0         # game info counter

# Perform a check for correct command structure
def check(tag, msg, state, curr, container, funct, conn, fail):
    if state == curr:
        for item in container:
            if re.match(item,msg):
                funct(tag,msg)
                conn.send("OK\n")
                return True
        conn.send("ILLEGAL\n;%s\n" % fail)
    return False

# Run the game for an individual connection as a thread
def run(conn):
    def game():

        # Assign a tag and start state machine
        global tag_count
        tag = tag_count
        tag_count += 1
        state = CHOOSESIDE
        conn.send("WELCOME\n")
        conn.setblocking(True)

        sys.stdout.write("Creating game %d\n" % tag)
        global game_states
        game_states[tag] = state_o()
        while True:
            
            # Receive a message and convert to uppercase
            try:
                msg = conn.recv(1024).strip()
                msg = msg.upper()
            except socket.error:
                continue

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
                print game_states[tag]
                conn.send("OK\n")
                dodisplay(tag)
                continue

            # UNDO
            elif re.match(undo_r,msg):
                conn.send("OK\n")
                if state < GAMEPLAY:
                    state -= 1
                    if state < 0:
                        state = 0
                else:
                    doundo(tag)
                continue

            # CHOOSE SIDE: BLACK, WHITE
            if check(tag,msg,state,CHOOSESIDE,sides,setside,conn,"Must choose a side"):
                state += 1

            # CHOOSE MODE: HUMAN-AI, AI-AI
            elif check(tag,msg,state,CHOOSEMODE,gamemodes,setmode,conn,"Must choose a mode"):
                state += 1

            # CHOOSE DIFFICUTLY: EASY, MEDIUM, HARD
            elif check(tag,msg,state,CHOOSEDIFFICULTY,difficulties,setdifficulty,conn,"Must choose a difficulty"):
                state += 1

            # EXECUTE A MOVE
            elif state == GAMEPLAY:
                if (re.match(move_r,msg) and domove(tag,msg)):
                    conn.send("OK\n")
                else:
                    conn.send("ILLEGAL\n;Bad move\n")
            
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
