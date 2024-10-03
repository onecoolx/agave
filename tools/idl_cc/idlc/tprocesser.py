
import os
import sys
import json

from .ir import *
from .parser import *
from .codegen import *

def checkFile(file):
    if not file.endswith('.json'):
        file += ".json"
    #end if

    # check file
    if os.access(file, os.F_OK|os.R_OK):
       return file
    #end if
    
    inc_dir = os.getenv('CATEGORY_INC_DIR')
    inc_file = inc_dir + '/' +file

    # check file
    if os.access(inc_file, os.F_OK|os.R_OK):
       return inc_file
    #end ifr

    msg = "file does not exist or unreadable!"
    raise ParserError(msg, file)
#end check

def loadModelFromJson(file, symbols):
    jsonFile = checkFile(file)
    # read contents
    fp = open(jsonFile, "r")
    contents = fp.read()
    fp.close()
    # load json data
    try:
        jsonData = json.loads(contents)
    except ValueError as err:
        raise ParserError(str(err), jsonFile)
    #end try

    try:
        m = model(jsonData, symbols, file)
    except Exception as err:
        raise err
    #end try

    if jsonData.__contains__('use-model'):
        subModels = jsonData['use-model']
        for key, value in subModels.items():
            subFile = str(value['name'])
            try:
                sm = loadModelFromJson(subFile, symbols)
                m.addSubModel(sm)
            except Exception as err:
                raise ParserError(str(err), subFile)
            #end try
        #end for
    #end if
    return m
#end load

def parse(infile):
    s = symbol();
    m = loadModelFromJson(infile, s)
    return (m, s)
#end parse

def _checkModel(models):
    hasClass = False
    hasProduct = False
    props = models.properties()
    for p in props:
        if p.name == 'class':
            hasClass = True
        #end if
        if p.name == 'productid':
            hasProduct = True
        #end if
    #end if
    if hasClass == False or hasProduct == False:
        raise CodeGenError("Model has no \"class\" or \"productid\" property define!")
    #end if
#end check

def generate(models, symbols, outfile):
    _checkModel(models)
    codeGen = CodeGen(models, symbols, outfile)
    try:
        codeGen.generate()
    except Exception as err:
        if os.path.isfile(outfile):
           os.remove(outfile)
        #endif
        raise err
    #end try
    print ("Outfile: " + outfile + " generated.")
#end generate
