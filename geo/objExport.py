import maya.cmds as cmds

objects = cmds.ls(sl=True)

print objects

#for object in objects:
cmds.file("~/Specialist/geo/"+objects[0]+".obj",pr=1,typ="OBJexport",es=1, op="groups=0; ptgroups=0; materials=0; smoothing=0; normals=0")

