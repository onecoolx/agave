
import os
import sys
import getopt

import idlc.tprocesser as tprocesser

version = "0.5.0"

def usage():
    print ("Thing Model Code Convertor ({0})\n".format(version))
    print ("usage:")
    print ("    tmcc <options> <XXX>.json")
#end

def printHelp():
    print \
("""
Thing Model Code Convertor ({0})

Options:
  -h, --help                 display this help and exit
  -v, --version              output version information and exit
  -o, --output FILE          specify output file
""".format(version))
#end

def Main(argc, argv):
    if argc == 1:
        usage()
        return 1
    #end

    infile = ""
    outfile = ""

    try:
        opts, args = getopt.getopt(argv[1:], '-h-o:-v', ['help', 'output', 'version'])
    except getopt.GetoptError as err:
        print("Error: {0}".format(err))
        return 2
    #end try

    for opt, value in opts:
        if opt in ('-v', '--version'):
            print ("Version: "+version)
            return 0
        elif opt in ('-h', '--help'):
            printHelp()
            return 0
        elif opt in ('-o', '--output'):
            outfile = value
    #end for

    for item in args:
        if item.endswith('.json'):
            infile = item
            break
    #end for

    if len(infile) == 0:
        print("Error: no input json file!")
        return 2
    #end if

    if not os.access(infile, os.F_OK|os.R_OK):
        print("Error: {0} does not exist or unreadable!").format(infile)
        return 2
    #end if
    
    baseName = os.path.basename(infile)

    if len(outfile) == 0:
        outfile = os.path.splitext(baseName)[0] + '.js'
    #end if

    try:
        model, symbol = tprocesser.parse(infile)
    except tprocesser.ParserError as err:
        print("Error: {0}".format(err))
        return 2
    #end try

    try:
        tprocesser.generate(model, symbol, outfile) #generate code
    except tprocesser.CodeGenError as err:
        print("Error: {0}".format(err))
        return 2
    #end try

    return 0
#end main

# main
if __name__ == '__main__':
    sys.exit(Main(len(sys.argv), sys.argv))
