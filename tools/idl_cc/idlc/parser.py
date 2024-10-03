
internal_types = {
    'string': 0x01,
    'bool':   0x02,
    'int8':   0x03,
    'uint8':  0x04,
    'int16':  0x05,
    'uint16': 0x06,
    'int32':  0x07,
    'uint32': 0x08,
    'float':  0x09,
    'double': 0x10,
    'array':  0x11,
}

class ParserError(Exception):
    def __init__(self, msg, filename, opt=''):
        self.msg = 'File: {0} '.format(filename) + msg
        self.opt = opt
        Exception.__init__(self, msg, opt)
    #end init

    def __str__(self):
        return self.msg
    #end str
#end ParserError


class symbol:
    'symbol table'
    IdxBase = 100  # symbol index base

    def __init__(self):
        self._curIdx = symbol.IdxBase
        self._typeDefs = []
        self._capDefs = []
        self._notifyDefs = []
    #end init

    def _checkIsPodType(self, t):
        for n, v in internal_types.items():
            if n == t:
                return (True, v)
            #end if
        #end for
        return (False, 0)
    #end pod

    def _findSymbolsInList(self, slist, name):
        ret = False
        s = []
        for i in range(len(slist)):
            item = slist[i]
            if item['name'] == name:
                ret = True
                s.append(i)
            #end if
        #end for
        return (ret, s)
    #end find

    def _getSid(self):
        self._curIdx = self._curIdx + 1
        return self._curIdx
    #end sid

    def _genNewSymbol(self, slist, name, sname, capId):
        sid = self._getSid()
        s = {"name": name, "id": sid, "sname": sname, "capId": capId}
        slist.append(s)
        return sid
    #end gen

    def _getSymbolNameId(self, slist, name, capId):
        ret, idxs = self._findSymbolsInList(slist, name)
        if ret == True:
            for i in idxs:
                item = slist[i]
                if item['capId'] != capId:
                    # name conflict
                    if item['sname'] == item['name']:
                        item['sname'] = item['name'] + item['capId']
                    #end if
                else:
                    return item['id']
                #end if
            #end for
            # add new
            return self._genNewSymbol(slist, name, name + capId, capId)
        else:
            # new symbol
            return self._genNewSymbol(slist, name, name, capId)
        #end if
    #end proc

    def genNameId(self, name, capId, type):
        if type == 'typeDef':
            ret, idx = self._checkIsPodType(name)
            if ret == True:
                return idx
            #end if
            return self._getSymbolNameId(self._typeDefs, name, capId)
        elif type == 'capsDef':
            return self._getSymbolNameId(self._capDefs, name, capId)
        elif type == 'notifyDef':
            return self._getSymbolNameId(self._notifyDefs, name, capId)
        else:
            raise TypeError("Unknow type name \"{0}\" for generate symbols".format(type))
    #end gen

    def _getSymbolById(self, slist, id):
        for i in range(len(slist)):
            item = slist[i]
            if item['id'] == id:
                return item['sname']
            #end if
        #end for
        return ""
    #end get by idx

    def getNameById(self, id, type):
        if type == 'typeDef':
            return self._getSymbolById(self._typeDefs, id)
        elif type == 'capsDef':
            return self._getSymbolById(self._capDefs, id)
        elif type == 'notifyDef':
            return self._getSymbolById(self._notifyDefs, id)
        else:
            raise TypeError("Unknow type name \"{0}\" for get symbols".format(type))
    #end get name

    # for debug
    def _dumpSymbols(self):
        print ("type defines:")
        for i in range(len(self._typeDefs)):
            t = self._typeDefs[i]
            print (t)
        #end for

        print ("capability define:")
        for i in range(len(self._capDefs)):
            c = self._capDefs[i]
            print (c)
        #end for

        print ("notify define:")
        for i in range(len(self._notifyDefs)):
            n = self._notifyDefs[i]
            print (n)
        #end for

    #end dump
#end symbol table
