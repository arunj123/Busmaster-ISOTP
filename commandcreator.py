import pprint


def addr2Bytes(addr):
    addrbytes = []
    addrbytes.append(addr & 0xFF)

    addr = addr >> 8
    addrbytes.append(addr & 0xFF)

    addr = addr >> 8
    addrbytes.append(addr & 0xFF)

    addr = addr >> 8
    addrbytes.append(addr & 0xFF)
    
    return addrbytes

class ODT:
    MAXSIZE = 6
    
    def __init__(self, countTimeStamp=False):   
        self.l = []
        if countTimeStamp: #if entry is first in daq
            self.usedbytes = 4 # if first odt, then timestamp
        else:
            self.usedbytes = 0


    def insertentry(self, v, offset=0):
        
        # temperory variable to construct
        temp = {}
        temp['size'] = v['size'] - offset
        temp['addr'] = v['addr'] + offset
        temp['addr'] = addr2Bytes(temp['addr'])
        temp['info'] = {'offset': offset, 'var': v['name']}

        # if variable wont fit in this odt
        if ((self.usedbytes + temp['size']) > self.MAXSIZE):
            temp['size'] = self.MAXSIZE - self.usedbytes

        # insert if space available
        if temp['size'] > 0:
            self.usedbytes = self.usedbytes + temp['size']
            self.l.append(temp)
        
        return temp['size']


class DAQ:
    def __init__(self):
        self.topOdt = 0
        self.odts = []
        self.odts.append(ODT(countTimeStamp = True)) 
    
    def insertOdtEntry(self, odte):
        
        insertedBytes = 0
        while True:
            insertedBytes = insertedBytes + self.odts[self.topOdt].insertentry(odte, insertedBytes)

            # if not all bytes could be read using same odt
            if insertedBytes < odte['size']:

                # Add one more odt
                self.odts.append(ODT()) 
                self.topOdt = self.topOdt + 1
            else:
                break



events = {
 #   "1 ms": 3,
    "10 ms": 1,
    "100 ms": 2,
}

varlist = [
    {"size": 1, "name": "TestByte_000", "addr": 0x1BD054, "event": "10 ms"},
    {"size": 4, "name": "TestWord_040", "addr": 0x1BD0F4, "event": "100 ms"},
]


eventsortlist = {}
daqinfo = {}
for e in events:
    eventsortlist[e] = []
    daqinfo[e] = DAQ()

# Assign sorted event list to disctionary
for e in events:
    eventsortlist[e] = sorted([i for i in varlist if (i['event'] == e)], key=lambda kv: kv['size'], reverse= True)

    # for each odt element
    for odte in eventsortlist[e]:
        daqinfo[e].insertOdtEntry(odte)


# initialize event list
daqcommand_resp = []

# ALLOC_DAQ 
daqcommand_resp.append([0xD5, 0x00, len( daqinfo.keys() ), 0x00])

# For n = MIN_DAQ to MIN_DAQ+DAQ_COUNT-1 do  ALLOC_ODT  
for daq_i, daq in enumerate(daqinfo.keys()):
    daqcommand_resp.append([0xD4, 0x00, daq_i, 0x00, len(daqinfo[daq].odts) ])

    # For i = 0 to ODT_COUNT(n)-1 do  ALLOC_ODT_ENTRY
    for odt_i, odt in enumerate(daqinfo[daq].odts):
        daqcommand_resp.append([0xD3, 0x00, daq_i, 0x00, odt_i, len(odt.l)])



# For n = MIN_DAQ to MIN_DAQ+DAQ_COUNT-1 do 
for daq_i, daq in enumerate(daqinfo.keys()):
    # For i = 0 to ODT_COUNT(n)-1 do  ALLOC_ODT_ENTRY do
    for odt_i, odt in enumerate(daqinfo[daq].odts):

        for odte_i, odte in enumerate(odt.l):

            # SET_DAQ_PTR
            daqcommand_resp.append([0xE2, 0x00, daq_i, 0x00, odt_i, odte_i ])

            # WRITE_DAQ
            daqcommand_resp.append([0xE1, 0xFF, odte['size'], 0x00] + odte['addr'])


# For n = MIN_DAQ to MIN_DAQ+DAQ_COUNT-1 do  SET_DAQ_LIST_MODE, START_STOP_DAQ_LIST
for daq_i, daq in enumerate(daqinfo.keys()):
    daqcommand_resp.append([0xE0, 0x10, daq_i, 0x00, events[daq], 0x00, 0x01, 0x00 ])
    daqcommand_resp.append([0xDE, 0x02, daq_i, 0x00])
    
# For n = 0 to MAX_DAQ-1 do START_STOP_DAQ_LIST
for daq_i, daq in enumerate(daqinfo.keys()):
    # SET_DAQ_LIST_MODE
    daqcommand_resp.append([0xDE, 0x02, daq_i, 0x00])

    # GET_DAQ_CLOCK
    daqcommand_resp.append([0xDC])

    # START_STOP_SYNCH
    daqcommand_resp.append([0xDD, 0x01])

for cmd in daqcommand_resp:
    print(len(cmd), ",", ', '.join(["0x{:02X}".format(p) for p in cmd]), ",")

