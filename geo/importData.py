import maya.cmds as cmds

objectList = ["base", "lower", "upper", "hand"]

cmds.showHidden(objectList)

with open("/home/i7621149/Specialist/Out.txt") as f:
    content = f.readlines()

rotData = []
transData = []

for i in range(0, len(content)-3, 2):
    line1 = content[i].strip('\n')
    line2 = content[i+1].strip('\n')
    line1Data = line1.split()
    line1Data = map(float, line1Data)
    line2Data = line2.split()
    line2Data = map(float, line2Data)
    rotData.append(line1Data)
    transData.append(line2Data)

fpsData = content[-2].strip('\n')
print fpsData
fps = int(fpsData)
print content[-1]
col = map(float, content[-1].split())

print "fps", fps
dist = fps * 0.5
height = 3

objectRotations = []
objectTranslations = []
for i in range(len(objectList)):
    objectRotations.append([])
    objectTranslations.append([])

for i in range(len(rotData)):
    objectRotations[i%len(objectList)].append(rotData[i])

for i in range(len(transData)):
    objectTranslations[i%len(objectList)].append(transData[i])

print objectRotations
print objectTranslations

print objectList

copyList = []
print "generating copies"
for j in range(len(objectRotations[0])):
    trans = [0,0,0]
    for i in range(len(objectList)):

        spin = float(j)
        spin = spin/float(fps) * 360

        geo = cmds.duplicate(objectList[i])[0]
        cmds.rotate(0, spin, 0, geo)
        cmds.move(dist, 0, 0, geo, os=True, r=True)

        print (j*len(objectList))+i, "/", len(objectList)*len(objectRotations[0]), geo[0], trans
        cmds.move(trans[0], trans[1], trans[2], geo, os=True, r=True)
        cmds.rotate(objectRotations[i][j][0], objectRotations[i][j][1], objectRotations[i][j][2], geo, os=True, r=True)
        trans[0] += objectTranslations[i][j][0]
        trans[1] += objectTranslations[i][j][1]
        trans[2] += objectTranslations[i][j][2]

        copyList.append(geo)

CTRL = cmds.circle()
cmds.move(0,height,0, CTRL)
GRP = cmds.group(copyList, n="frames")
tableGEO = cmds.polyCylinder(r = dist * 1.2, h=0.5, sx = 100)
cmds.move(0, -0.25, 0, tableGEO)
tableGRP = cmds.group(tableGEO, n="table")

#should be multiple of 10
spinNum = 100

cmds.parentConstraint(CTRL, GRP, maintainOffset = True)
cmds.parentConstraint(CTRL, tableGRP, maintainOffset=True)
cmds.currentTime(0)
cmds.setKeyframe(CTRL)

cmds.currentTime(fps*spinNum)
cmds.rotate(0,360*spinNum,0, CTRL)

cmds.setKeyframe(CTRL)
cmds.setAttr("lambert2.color", col[0], col[1], col[2])
cmds.hide(objectList)

cmds.group(CTRL, tableGRP, GRP, n="zoetrope")
