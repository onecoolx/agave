
import os

from .parser import *
from .ir import *

class CodeGenError(Exception):
    def __init__(self, msg, opt=''):
        self.msg = 'Code Generator Error: ' + msg
        self.opt = opt
        Exception.__init__(self, msg, opt)
    #end init

    def __str__(self):
        return self.msg
    #end str
#end CodeGenError

class CodeGen:
    'JavaScript Codegen'
    def __init__(self, model, symbols, outfile):
        self._apiVersion = '0x01'
        self._stringType = 0x01
        self._boolType = 0x02
        self._int8Type = 0x03
        self._uint8Type = 0x04
        self._int16Type = 0x05
        self._uint16Type = 0x06
        self._int32Type = 0x07
        self._uint32Type = 0x08
        self._floatType = 0x09
        self._doubleType = 0x10
        self._arrayType = 0x11
        self._numberTypeMax = self._doubleType
        self._enumType = self._uint32Type
        self._npodTypeBase = 100
        self._model = model
        self._symbols = symbols
        self._outfile = outfile
    #end init

    def generate(self):
        if os.path.isfile(self._outfile):
            os.remove(self._outfile)
        #endif

        className = self._className() # get class name

        fp = open(self._outfile, "a+")
        self._writeHeader(fp)
        self._writeTypeDefines(fp, self._model)
        self._writeClassDefines(fp, className)
        self._writeEnumDefines(fp, className, self._model)
        self._writeExports(fp, className, self._model)
        fp.write('\n')
        fp.write('module.exports = '+className+';\n')
        fp.close()
    #end gen

    def _writeHeader(self, file):
        file.write('\"use strict\";\n')
        file.write('\n')
        file.write('const sdk = require(\"@ali/aiot-core-sdk\");\n')
        file.write('const AIOTError = sdk.AIOTError;\n')
        file.write('\n')
    #end write

    def _className(self):
        properties = self._model.properties()
        for p in properties:
            if p.name == 'class':
                return p.value
            #end if
        #end for
        raise CodeGenError("No property \"class\" is found!")
    #end class name

    def _isRange(self, typeId, model):
        ranges = model.ranges()
        for r in ranges:
            if r.nameIdx == typeId:
                return True
            #end if
        #end for
        return False
    #end is range

    def _getTypeDefById(self, typeId, model):
        types = model.typeDefines()
        for t in types:
            if t.nameIdx == typeId:
                return t
            #end of
        #end for
        raise CodeGenError("No type define is found!")
    #end type by id

    def _getDefaultValByType(self, typeId, model):
        if typeId == self._stringType:
            return '\"\"'
        elif typeId == self._boolType:
            return 'false'
        elif typeId >= 0x03 and typeId <= self._numberTypeMax: 
            return '0'
        elif typeId == self._arrayType:
            return '[]'
        elif self._isRange(typeId, model):
            return '0'
        else:
            return 'null'
    #end get defval

    def _writeTypeDefines(self, file, model):
        types = model.typeDefines()
        for v in types:
            if (v.type == "struct"):
                name = self._symbols.getNameById(v.nameIdx, "typeDef")
                file.write('//'+name+'\n')
                file.write('class '+name+' {\n');
                file.write('\tconstructor() {\n')
                # struct fields
                fields = v.fields
                for f in fields:
                    defVal = self._getDefaultValByType(f['typeIdx'], model)
                    file.write('\t\tthis.'+f['name']+' = '+defVal+';\n') 
                #end for
                file.write('\t}\n')
                file.write('}\n')
                file.write('\n')
            #end if
        #end for

        subModels = model.subModels()
        for s in subModels:
            self._writeTypeDefines(file, s)
        #end for
    #end typedefs

    def _writeEnumDefines(self, file, className, model):
        ranges = model.ranges()
        for r in ranges:
            name = self._symbols.getNameById(r.nameIdx, "typeDef")
            file.write(className+'.'+name+' = {\n')
            for e in r.enums:
                file.write('\t')
                for k, v in e.items():
                    file.write(k+': '+str(v)+',')
                #end for
                file.write('\n')
            #end for
            file.write('}\n')
            file.write('\n')
        #end for

        subModels = model.subModels()
        for s in subModels:
            self._writeEnumDefines(file, className, s)
        #end for
    #end enum

    def _writeClassDefines(self, file, className):
        file.write('class '+className+' {\n')
        file.write('\n')
        # common methods
        file.write('\tconstructor() {\n')
        file.write('\t\tthis._thing = new sdk.ThingModelProxy();\n')
        file.write('\t}\n\n')
        file.write('\t// statusCallback : (status) => {}\n')
        file.write('\t// retCallback : (code) => {}\n')
        file.write('\tbindDevice(device, statusCallback, retCallback) {\n')
        file.write('\t\tthis._thing.bindTo(device, statusCallback, retCallback);\n')
        file.write('\t}\n\n')
        file.write('\t// retCallback : (code) => {}\n')
        file.write('\treleaseDevice(retCallback) {\n')
        file.write('\t\tthis._thing.unbindTo(retCallback);\n')
        file.write('\t}\n\n')
        # properties
        self._writeProperties(file)
        self._writeCapabilities(file, self._model)
        file.write('}\n')
        file.write('\n')
    #end class

    def _writeProperties(self, file):
        props = self._model.properties()
        for p in props:
            file.write('\tget '+p.name+'() {\n')
            file.write('\t\treturn \"'+p.value+'\";\n')
            file.write('\t}\n\n')
        #end for
    #end write prop

    def _writeParam(self, file, holder, name, typeId):
        if typeId == self._stringType:
            file.write(holder+'.writeString('+name+');\n')
        elif typeId == self._int32Type or typeId == self._uint32Type:
            file.write(holder+'.writeInt32('+name+');\n')
        elif typeId == self._int16Type or typeId == self._uint16Type:
             file.write(holder+'.writeInt16('+name+');\n')   
        elif typeId == self._int8Type or typeId == self._uint8Type:
             file.write(holder+'.writeInt8('+name+');\n')
        elif typeId == self._floatType or typeId == self._doubleType:
             file.write(holder+'.writeDouble('+name+');\n')
        elif typeId == self._boolType:
             file.write(holder+'.writeInt8('+name+'? 1 : 0);\n')
        #end if
    #end write param

    def _writeParams(self, file, holder, name, typeId, model):
        if typeId <= self._numberTypeMax: #pod type
            file.write('\t\t')
            self._writeParam(file, holder, name, typeId)
        elif self._isRange(typeId, model):
            file.write('\t\t')
            self._writeParam(file, holder, name, self._enumType)
        else:
            td = self._getTypeDefById(typeId, model)
            if td.type == 'struct':
                for f in td.fields:
                    self._writeParams(file, holder, name+"."+f['name'], f['typeIdx'], model)
                #end for
            else: # typedef
                file.write('\t\t')
                self._writeParam(file, holder, name, td.typeNameIdx)
            #end if
        #end if
    #end write params

    def _writeReadResult(self, file, holder, name, typeId):
        if typeId == self._stringType:
            file.write(name+' = '+holder+'.readString();\n')
        elif typeId == self._int32Type or typeId == self._uint32Type:
            file.write(name+' = '+holder+'.readInt32();\n')
        elif typeId == self._int16Type or typeId == self._uint16Type:
             file.write(name+' = '+holder+'.readInt16();\n')   
        elif typeId == self._int8Type or typeId == self._uint8Type:
             file.write(name+' = '+holder+'.readInt8();\n')
        elif typeId == self._floatType or typeId == self._doubleType:
             file.write(name+' = '+holder+'.readDouble();\n')
        elif typeId == self._boolType:
             file.write(name+' = '+holder+'.readInt8() == 1 ? true : false;\n')
        #end if
    #end write params

    def _writeReadResults(self, file, holder, name, typeId, model):
        if typeId <= self._numberTypeMax: #pod type
            self._writeReadResult(file, holder, name, typeId)
        elif self._isRange(typeId, model):
            self._writeReadResult(file, holder, name, self._enumType)
        else:
            td = self._getTypeDefById(typeId, model)
            if td.type == 'struct':
                tname = self._symbols.getNameById(typeId, "typeDef")
                file.write(name+' = new '+tname+'();\n')
                for f in td.fields:
                    file.write('\t\t\t')
                    self._writeReadResults(file, holder, name+"."+f['name'], f['typeIdx'], model)
                #end for
            else: # typedef
                self._writeReadResult(file, holder, name, td.typeNameIdx)
            #end if
        #end if
    #end result

    def _writeCapabilities(self, file, model):
        if len(model.categoryId()) > 0:
            file.write('\t// '+model.name()+'\n')
            file.write('\t// categoryid : '+model.categoryId()+'\n\n')
        #end if

        caps = model.capabilities()
        for c in caps:
            name = self._symbols.getNameById(c.nameIdx, "capsDef")
            if c.isProperty == False: # is function
                file.write('\t// '+name+'\n')
                file.write('\t// id : '+str(c.id)+'\n')
                for i in c.inputs:
                    typeName = self._symbols.getNameById(i['typeIdx'], "typeDef")
                    file.write('\t// '+i['name']+': '+typeName+'\n')
                #end for

                file.write('\t// retCallback : (code')
                for o in c.outputs:
                    if o['name'] == 'code':
                        continue
                    #end if

                    file.write(', '+o['name'])
                    typeId = o['typeIdx']
                    if typeId > self._npodTypeBase: # not pod type
                        typeName = self._symbols.getNameById(o['typeIdx'], "typeDef")
                        file.write(' <'+typeName+'>')
                    #end if
                #end for

                file.write(') => {}\n')
                file.write('\t'+name+'(')
                for i in c.inputs:
                    file.write(i['name']+', ')
                #end for
                file.write('retCallback) {\n')
                file.write('\t\tlet msg = new sdk.Message();\n')
                file.write('\t\tmsg.writeInt16({0});\n'.format(self._apiVersion))
                optId = str(hex((int(model.categoryId()) << 6) | c.id))
                file.write('\t\tmsg.writeInt16({0});\n'.format(optId))
                for p in c.inputs:
                    self._writeParams(file, "msg", p['name'], p['typeIdx'], model)
                #end for
                file.write('\t\tthis._thing.invokeCapablility('+optId+', msg, (err, id, rmsg) => {\n')

                for q in c.outputs:
                    if q['name'] == 'code':
                        continue
                    #end if
                    file.write('\t\t\tlet ')
                    self._writeReadResults(file, "rmsg", q['name'], q['typeIdx'], model)
                #end for

                file.write('\t\t\tretCallback(err')
                for q in c.outputs:
                    if q['name'] == 'code':
                        continue
                    #end if
                    file.write(', '+q['name'])
                #end for
                file.write(');\n')
                file.write('\t\t}, (err) => {\n')
                file.write('\t\t\tif (err !== AIOTError.Code.AiotOk) {\n')
                file.write('\t\t\t\tretCallback(err);\n')
                file.write('\t\t\t}\n')
                file.write('\t\t});\n')
                file.write('\t}\n')
                file.write('\n')
            else:  # is property
                if len(c.getters) > 0:
                    g = c.getters[0]
                    file.write('\t// get_'+name+'\n')
                    file.write('\t// id : '+str(c.id)+'\n')
                    file.write('\t// callback : (code')
                    file.write(', '+g['name'])

                    typeId = g['typeIdx']
                    if typeId > self._npodTypeBase: # not pod type
                        typeName = self._symbols.getNameById(g['typeIdx'], "typeDef")
                        file.write(' <'+typeName+'>')
                    #end if

                    file.write(') => {}\n')
                    file.write('\tget_'+name+'(callback) {\n')
                    file.write('\t\tlet msg = new sdk.Message();\n')
                    file.write('\t\tmsg.writeInt16({0});\n'.format(self._apiVersion))
                    optId = str(hex((int(model.categoryId()) << 6) | c.id))
                    file.write('\t\tmsg.writeInt16({0});\n'.format(optId))
                    file.write('\t\tthis._thing.invokeCapablility('+optId+', msg, (err, id, rmsg) => {\n')
                    file.write('\t\t\tlet ')
                    self._writeReadResults(file, "rmsg", g['name'], g['typeIdx'], model)
                    file.write('\t\t\tcallback(err, {0});\n'.format(g['name']))
                    file.write('\t\t}, (err) => {\n')
                    file.write('\t\t\tif (err !== AIOTError.Code.AiotOk) {\n')
                    file.write('\t\t\t\tcallback(err);\n')
                    file.write('\t\t\t}\n')
                    file.write('\t\t});\n')
                    file.write('\t}\n')
                    file.write('\n')
                #end if

                if len(c.setters) > 0:
                    s = c.setters[0]
                    file.write('\t// set_'+name+'\n')
                    file.write('\t// id : '+str(c.id)+'\n')

                    typeId = s['typeIdx']
                    if typeId > self._npodTypeBase: # not pod type
                        typeName = self._symbols.getNameById(s['typeIdx'], "typeDef")
                        file.write('\t// '+s['name'] +' : <'+typeName+'>\n')
                    #end if

                    file.write('\t// callback : (code) => {}\n')
                    file.write('\tset_'+name+'(')
                    file.write(s['name']+', ')
                    file.write('callback) {\n')
                    file.write('\t\tlet msg = new sdk.Message();\n')
                    file.write('\t\tmsg.writeInt16({0});\n'.format(self._apiVersion))
                    optId = str(hex((int(model.categoryId()) << 6) | c.id))
                    file.write('\t\tmsg.writeInt16({0});\n'.format(optId))
                    self._writeParams(file, "msg", s['name'], s['typeIdx'], model)
                    file.write('\t\tthis._thing.invokeCapablility('+optId+', msg, (err, id, rmsg) => {\n')
                    file.write('\t\t\tcallback(err);\n')
                    file.write('\t\t}, (err) => {\n')
                    file.write('\t\t\tif (err !== AIOTError.Code.AiotOk) {\n')
                    file.write('\t\t\t\tcallback(err);\n')
                    file.write('\t\t\t}\n')
                    file.write('\t\t});\n')
                    file.write('\t}\n')
                    file.write('\n')
                #end if
                #end if
            #end if
        #end for

        # write notifications
        self._writeNotifications(file, model)

        subModels = model.subModels()
        for s in subModels:
            self._writeCapabilities(file, s)
        #end for
    #end write caps

    def _writeNotifications(self, file, model):
        notifs = model.notifications()
        for n in notifs:
            name = self._symbols.getNameById(n.nameIdx, "notifyDef")

            file.write('\t// '+name+'\n')
            file.write('\t// id : '+str(n.id)+'\n')
            file.write('\t// channel : '+str(n.channel)+'\n')
            file.write('\n')
            file.write('\t// callback : (')
            idx = 0
            for d in n.data:
                if idx > 0:
                    file.write(', ')
                #end if
                file.write(d['name'])
                typeId = d['typeIdx']
                if typeId > self._npodTypeBase: # not pod type
                    typeName = self._symbols.getNameById(d['typeIdx'], "typeDef")
                    file.write(' <'+typeName+'>')
                #end if
                idx = idx + 1
            #end for
            file.write(') => {}\n')
            file.write('\t// retCallback : (code) => {}\n')
            file.write('\ton_'+name+'(callback, retCallback) {\n')
            file.write('\t\tlet option = new sdk.OptionParam();\n')
            file.write('\t\toption.setChannelId({0});\n'.format(n.channel))

            optId = str(hex((int(model.categoryId()) << 6) | n.id))
            file.write('\t\tthis._thing.registerNotification('+optId+', option, (id, msg) => {\n')

            for d in n.data:
                file.write('\t\t\tlet ')
                self._writeReadResults(file, "msg", d['name'], d['typeIdx'], model)
            #end for

            file.write('\t\t\tcallback(')
            idx = 0
            for d in n.data:
                if idx > 0:
                    file.write(', ')
                #end if
                file.write(d['name'])
                idx = idx + 1
            #end forr
            file.write(');\n')
            file.write('\t\t}, retCallback);\n')
            file.write('\t}\n')
            file.write('\n')
            file.write('\t// retCallback : (code) => {}\n')
            file.write('\toff_'+name+'(retCallback) {\n')
            optId = str(hex((int(model.categoryId()) << 6) | n.id))
            file.write('\t\tthis._thing.unregisterNotification('+optId+', retCallback);\n')
            file.write('\t}\n')
            file.write('\n')
        #end for
    #end notify

    def _writeExports(self, file, className, model):
        types = model.typeDefines()
        for v in types:
            if (v.type == "struct"):
                name = self._symbols.getNameById(v.nameIdx, "typeDef")
                file.write(className+'.'+name+' = '+name+';\n')
            #end if
        #end for
        subModels = model.subModels()
        for s in subModels:
            self._writeExports(file, className, s)
        #end for
    #end export
