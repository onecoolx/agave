class base:
    def checkType(self, value, typeName):
        if type(value).__name__ != typeName:
            raise TypeError(self.__doc__ + " value type is not {0}".format(typeName))
        #end if
    #end check

    def checkStrType(self, value):
        if type(value).__name__ != 'str' and type(value).__name__ != 'unicode':
            raise TypeError(self.__doc__ + " value type is not string")
        #end if
    #end check
#end base

class prop(base):
    'property'
    def __init__(self, name, value):
        self.checkStrType(name)
        self.checkStrType(value)

        self.name = name
        self.value = value
    #end init
#end prop

class range(base):
    'range'
    def __init__(self, name, value, symbols, capId):
        self.checkStrType(name)
        self.checkStrType(capId)
        self.checkType(value, 'list')

        self.nameIdx = symbols.genNameId(name, capId, "typeDef")
        if len(value) == 0:
            raise TypeError("range \"{0}\" can not be empty!".format(name))
        #endif
        self.checkType(value[0], 'dict')
        self.enums = value;
    #end init
#end range

class caps(base):
    'capabilities'
    def __init__(self, name, value, symbols, capId):
        self.checkStrType(name)
        self.checkStrType(capId)
        self.checkType(value, 'dict')

        self.nameIdx = symbols.genNameId(name, capId, "capsDef")
        if not value.__contains__('id'):
            raise TypeError("capability \"{0}\" need \"id\" field!".format(name))
        #end if
        self.id = value['id']
        self.isProperty = False
        if value.__contains__('isProperty'):
            if value['isProperty'] == 'true':
                self.isProperty = True
            #end if
        #end if

        if (self.isProperty == False): # mathod
            self.outputs = []
            if not value.__contains__('output'):
                raise TypeError("capability \"{0}\" is not property so need \"output\" field!".format(name))
            #end if
            item = value['output']

            if type(item).__name__ == 'dict':
                if not item.__contains__('name') or not item.__contains__('type'):
                    raise TypeError("output field must has \"name\" and \"type\"!")
                #end if
                typeIdx = symbols.genNameId(item['type'], capId, "typeDef")
                self.outputs.append({"name": item['name'], "typeIdx": typeIdx})
            elif type(item).__name__ == 'list':
                for it in item:
                    if not it.__contains__('name') or not it.__contains__('type'):
                        raise TypeError("output field must has \"name\" and \"type\"!")
                    #end if
                    typeIdx = symbols.genNameId(it['type'], capId, "typeDef")
                    self.outputs.append({"name": it['name'], "typeIdx": typeIdx})
                #end for
            else:
                raise TypeError("output field must be \"dict\" or \"list\"!")
            #end if

            self.inputs = []
            if value.__contains__('input'):
                item = value['input']
                if type(item).__name__ == 'dict':
                    if not item.__contains__('name') or not item.__contains__('type'):
                        raise TypeError("input field must has \"name\" and \"type\"!")
                    #end if
                    typeIdx = symbols.genNameId(item['type'], capId, "typeDef")
                    self.inputs.append({"name": item['name'], "typeIdx": typeIdx})
                elif type(item).__name__ == 'list':
                    for it in item:
                        if not it.__contains__('name') or not it.__contains__('type'):
                            raise TypeError("output field must has \"name\" and \"type\"!")
                        #end if
                        typeIdx = symbols.genNameId(it['type'], capId, "typeDef")
                        self.inputs.append({"name": it['name'], "typeIdx": typeIdx})
                    #end for
                else:
                    raise TypeError("input field must be \"dict\" or \"list\"!")
                #end if
            #end if
        else: # property
            self.getters = []
            if value.__contains__('getter'):
                item = value['getter']
                if type(item).__name__ == 'dict':
                    if not item.__contains__('name') or not item.__contains__('type'):
                        raise TypeError("getter field must has \"name\" and \"type\"!")
                    #end if
                    typeIdx = symbols.genNameId(item['type'], capId, "typeDef")
                    self.getters.append({"name": item['name'], "typeIdx": typeIdx})
                elif type(item).__name__ == 'list':
                    for it in item:
                        if not it.__contains__('name') or not it.__contains__('type'):
                            raise TypeError("getter field must has \"name\" and \"type\"!")
                        #end if
                        typeIdx = symbols.genNameId(it['type'], capId, "typeDef")
                        self.getters.append({"name": it['name'], "typeIdx": typeIdx})
                    #end for
                else:
                    raise TypeError("getter field must be \"dict\" or \"list\"!")
                #end if
            #end if

            self.setters = []
            if value.__contains__('setter'):
                item = value['setter']
                if type(item).__name__ == 'dict':
                    if not item.__contains__('name') or not item.__contains__('type'):
                        raise TypeError("setter field must has \"name\" and \"type\"!")
                    #end if
                    typeIdx = symbols.genNameId(item['type'], capId, "typeDef")
                    self.setters.append({"name": item['name'], "typeIdx": typeIdx})
                elif type(item).__name__ == 'list':
                    for it in item:
                        if not it.__contains__('name') or not it.__contains__('type'):
                            raise TypeError("setter field must has \"name\" and \"type\"!")
                        #end if
                        typeIdx = symbols.genNameId(it['type'], capId, "typeDef")
                        self.setters.append({"name": it['name'], "typeIdx": typeIdx})
                    #end for
                else:
                    raise TypeError("setter field must be \"dict\" or \"list\"!")
                #end if
            #end if
        #end if
    #end init
#end caps

class notify(base):
    'notifications'
    def __init__(self, name, value, symbols, capId):
        self.checkStrType(name)
        self.checkStrType(capId)
        self.checkType(value, 'dict')

        self.nameIdx = symbols.genNameId(name, capId, "notifyDef")
        if not value.__contains__('id') or not value.__contains__('channel'):
            raise TypeError("notification \"{0}\" need \"id\" and \"channel\" field!".format(name))
        #end if
        self.id = value['id']
        self.channel = value['channel']
        self.data = []
        if not value.__contains__('data'):
            raise TypeError("notification \"{0}\" need \"data\" field!".format(name))
        #end if

        item = value['data']
        if type(item).__name__ == 'dict':
            if not item.__contains__('name') or not item.__contains__('type'):
                raise TypeError("data field must has \"name\" and \"type\"!")
            #end if
            typeIdx = symbols.genNameId(item['type'], capId, "typeDef")
            self.data.append({"name": item['name'], "typeIdx": typeIdx})
        elif type(item).__name__ == 'list':
            for it in item:
                if not it.__contains__('name') or not it.__contains__('type'):
                    raise TypeError("data field must has \"name\" and \"type\"!")
                #end if
                typeIdx = symbols.genNameId(it['type'], capId, "typeDef")
                self.data.append({"name": it['name'], "typeIdx": typeIdx})
            #end for
        else:
            raise TypeError("data field must be \"dict\" or \"list\"!")
        #end if
    #end init
#end notify

class typed(base):
    'type-define'
    def __init__(self, name, value, symbols, capId):
        self.checkStrType(name)
        self.checkStrType(capId)

        self.nameIdx = symbols.genNameId(name, capId, "typeDef")
        if type(value).__name__ == "list": # struct
            self.type = 'struct'
            self.fields = []
            for item in value:
                self.checkType(item, 'dict')
                if not item.__contains__('type') or not item.__contains__('name'):
                    raise TypeError("type-define \"{0}\" need \"type\" or \"name\" field!".format(name))
                #end if
                valType = item['type']
                valName = item['name']
                valTypeIdx = symbols.genNameId(valType, capId, "typeDef")
                if item.__contains__('range'): # is enum
                    valTypeIdx = symbols.genNameId(item['range'], capId, "typeDef")
                #end if
                field = {"name": valName, "typeIdx": valTypeIdx}
                self.fields.append(field)
            #end for
        elif type(value).__name__ == "dict": # typedef
            self.type = 'typedef'
            if not value.__contains__('type'):
                raise TypeError("type-define \"{0}\" need \"type\" field!".format(name))
            #end if
            valType = value['type']
            self.typeNameIdx = symbols.genNameId(valType, capId, "typeDef")

            if value.__contains__('range'): # enum
                self.type = 'enum'
                rangeType = value['range']
                self.rangeNameIdx = symbols.genNameId(rangeType, capId, "typeDef")
            #end if
        else:
            raise TypeError("Invalid type-define of \"{0}\"!".format(name))
        #end if
    #end init
#end typed

# thing model object
class model(base):
    'model'
    def __init__(self, obj, symbols, name):
        self._categoryid = ""
        self._name = name
        self._children = []
        self._properties = []
        self._typeDefines = []
        self._ranges = []
        self._capabilities = []
        self._notifications = []
        self._process(obj, symbols)
    #end init

    def _process(self, obj, symbols):
        self._processProperty(obj, symbols)
        self._processTypeDefines(obj, symbols)
        self._processRanges(obj, symbols)
        self._processCapabilities(obj, symbols)
        self._processNotification(obj, symbols)
    #end process

    def _processProperty(self, obj, symbols):
        if obj.__contains__('property'):
            items = obj['property']
            self.checkType(items, 'dict')
            for key, value in items.items():
                p = prop(key, value)
                self._properties.append(p)
                # find categoryid
                if key == 'categoryid':
                    self._categoryid = str(value)
            #end for
        #end if
    #end property

    def _processTypeDefines(self, obj, symbols):
        if obj.__contains__('type-define'):
            items = obj['type-define']
            self.checkType(items, 'dict')
            for key, value in items.items():
                t = typed(key, value, symbols, self._categoryid)
                self._typeDefines.append(t)
            #end for
        #end if
    #end type define

    def _processRanges(self, obj, symbols):
        if obj.__contains__('range'):
            items = obj['range']
            self.checkType(items, 'dict')
            for key, value in items.items():
                r = range(key, value, symbols, self._categoryid)
                self._ranges.append(r)
            #end for
        #end if
    #end range

    def _processCapabilities(self, obj, symbols):
        if obj.__contains__('capabilities'):
            items = obj['capabilities']
            self.checkType(items, 'dict')
            for key, value in items.items():
                c = caps(key, value, symbols, self._categoryid)
                self._capabilities.append(c)
            #end for
        #end if
    #end caps

    def _processNotification(self, obj, symbols):
        if obj.__contains__('notifications'):
            items = obj['notifications']
            self.checkType(items, 'dict')
            for key, value in items.items():
                n = notify(key, value, symbols, self._categoryid)
                self._notifications.append(n)
            #end for
        #end if
    #end notify

    def addSubModel(self, subModel):
        scapId = subModel.categoryId()
        for s in self._children:
            if scapId == s.categoryId():
                raise TypeError("categoryId conflict! too many model has categoryId: "+scapId)
            #end if
        #end for
        self._children.append(subModel)
    #end add

    def name(self):
        return self._name
    #end name

    def categoryId(self):
        return self._categoryid
    #end capid

    def subModels(self):
        return self._children
    #end children

    def properties(self):
        return self._properties
    #end properties

    def typeDefines(self):
        return self._typeDefines
    #end type defines

    def ranges(self):
        return self._ranges
    #end ranges

    def capabilities(self):
        return self._capabilities
    #end caps

    def notifications(self):
        return self._notifications
    #end notify
#end model
