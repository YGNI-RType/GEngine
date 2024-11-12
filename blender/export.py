import bpy
import os
import numpy as np
from mathutils import Matrix, Vector, Euler
import math
from copy import deepcopy


def origin_to_bottom(ob, matrix=Matrix()):
    me = ob.data
    mw = ob.matrix_world
    local_verts = [matrix @ Vector(v[:]) for v in ob.bound_box]
    o = sum(local_verts, Vector()) / 8
    o.z = min(v.z for v in local_verts)
    o = matrix.inverted() @ o
    me.transform(Matrix.Translation(-o))
    mw.translation = mw @ o

def load_euler_angles(file_path):
    try:
        with open(file_path, 'r') as file:
            # Read the first line and split by commas
            line = file.readline().strip()
            euler_values = [float(val) for val in line.split(' ')]

            # Ensure the file has 3 values for the Euler angles (XYZ)
            if len(euler_values) == 3:
                return euler_values
            else:
                raise ValueError("The file should contain exactly 3 values for Euler angles.")
    except Exception as e:
        print(f"Error reading file: {e}")
        return None

output_directory = "/home/epitech/Documents/very_mdel_rtype/export-al/exported3/"

if not os.path.exists(output_directory):
    os.makedirs(output_directory)

bpy.ops.object.select_all(action='DESELECT')
already_seen = {}
target_rotation = Euler((0, 0, 0))

for collection in bpy.data.collections:
    for object in collection.all_objects:
        if (object.type != 'MESH'):
            continue
        if (object.name.isdigit()):
            continue

        splitted = object.name.split('.')
        realname = splitted[0]

        alreadyRegisterd = realname in already_seen.keys()

        if alreadyRegisterd:
            continue

        object.select_set(True)
        already_seen[realname] = object
        obj_file_path = os.path.join(output_directory, f"{realname}.obj")
        info_file = os.path.join(output_directory, f"{realname}.info")

        origin_to_bottom(object)
        # bpy.ops.object.origin_set(type='ORIGIN_CENTER_OF_VOLUME')
        bpy.ops.object.location_clear(clear_delta=False)
        ogAngle = load_euler_angles(info_file)
        if (ogAngle == None):
            object.select_set(False)
            continue
        object.rotation_mode = "ZXY"
        object.rotation_euler = (math.radians(ogAngle[0]), math.radians(ogAngle[1]), math.radians(ogAngle[2]))

        bpy.ops.wm.obj_export(filepath=obj_file_path, export_selected_objects=True, export_materials=True)
        # bpy.ops.export_scene.gltf(filepath=obj_file_path, export_format='GLB', export_materials='EXPORT', use_selection=True)
        object.select_set(False)

print("Export completed. Check the log file for details.")