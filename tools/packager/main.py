import struct
import json
from PIL import Image
import sys

TAG_MAP = {
    "IDLE": 0,
    "WALK": 1,
    "RUN": 2,
    "INTERACT": 3,
    "DANCE": 4,
    "SIT": 5,
    "SLEEP": 6,
    "HIGHLIGHT": 7
}

HEADER_FMT = '<4sBB'
CHARDATA_FMT = '<32sBB'
ENTRY_FMT = '<BBBII4b'

def convert_image(imagePath):
    img = Image.open(imagePath).convert('RGBA')
    rgb565 = []
    for r, g, b, a in img.getdata():
        
        if a < 128: 
            r, g, b = 255, 0, 255
            
        r_5bit = (r >> 3) & 0x1F
        g_6bit = (g >> 2) & 0x3F
        b_5bit = (b >> 3) & 0x1F
        
        dt = (r_5bit << 11) | (g_6bit << 5) | b_5bit
        rgb565.append(dt)
    return struct.pack(f'<{len(rgb565)}H', *rgb565)

def package_dude_file(srcPath, outputPath):
    with open(f'{srcPath}/data.json', 'r') as file:
        charData = json.load(file)
    
    with open(f'{srcPath}/animations.json', 'r') as file:
        animations = json.load(file)
    
    entries = []
    imgBlob = bytearray()
    
    dataOffset = struct.calcsize(HEADER_FMT) + struct.calcsize(CHARDATA_FMT) + (struct.calcsize(ENTRY_FMT) * len(animations))
    
    for anim in animations:
        
        tags = [TAG_MAP.get(t.upper(), -1) for t in anim['tag']]
        while len(tags) < 4:
            tags.append(-1)
        tags = tags[:4]
    
        data = convert_image(f"{srcPath}/sprites/{anim['source']}")

        entry = struct.pack(ENTRY_FMT,
            anim['frameCount'],
            anim['frameWidth'],
            anim['frameHeight'],
            dataOffset,
            len(data),
            *tags
        )
        
        imgBlob.extend(data)
        entries.append(entry)
        dataOffset += len(data)
        
    with open(outputPath, 'wb') as file:
        file.write(struct.pack(HEADER_FMT, b'DUD3',1, len(animations)))
        file.write(struct.pack(CHARDATA_FMT, charData['name'].encode('utf-8'), charData['baseSpeed'], charData['runSpeed']))
        for entry in entries:
            file.write(entry)
        file.write(imgBlob)
    print("File created on " + output)
        
if len(sys.argv) < 3:
    print("Usage: python main.py <source> <output>")
    sys.exit(1)
    
source = sys.argv[1]
output = sys.argv[2]
    
package_dude_file(source,output)
# package_dude_file("../../assets/sample","../../assets/sample.dude")