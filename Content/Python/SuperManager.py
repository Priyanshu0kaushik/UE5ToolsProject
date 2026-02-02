import unreal

EAL = unreal.EditorAssetLibrary
PREFIX_MAPPING = {
    "StaticMesh": "SM_",
    "Texture2D": "T_",
    "Material": "M_",
    "MaterialInstanceConstant": "MI_",
    "Blueprint": "BP_"
}

def generate_report():
    assets_paths = EAL.list_assets("/Game", True, True)
    for asset_path in assets_paths:
        if EAL.does_directory_exist(asset_path):
            if not EAL.does_directory_have_assets(asset_path):
                print(f"CLEANUP: Empty Folder found at {asset_path}")
            continue
            
        asset = EAL.find_asset_data(asset_path)
        class_path = asset.get_class()
        class_name = str(class_path.get_class_path_name().asset_name)
        asset_name = str(asset.asset_name)
        check_prefex(asset_name, class_name)
        if(class_name == "Texture2D"):
            check_texture_optimation(asset)
        elif(class_name == "StaticMesh"):
            check_mesh_collision(asset)
        
                
def check_prefex(asset_name, class_name):
    
    expected_prefix = PREFIX_MAPPING.get(class_name)
        
    if expected_prefix:
        if not asset_name.startswith(expected_prefix):
            print("Wrong Prefix Class: "+ class_name + " Asset Name: " + asset_name)
            
def check_is_power_of_two(n):
    return n > 0 and (n & (n - 1)) == 0


def check_texture_optimation(asset):
    asset_obj = asset.get_asset()
    x = asset_obj.blueprint_get_size_x()
    y = asset_obj.blueprint_get_size_y()
    
    if not check_is_power_of_two(x) or not check_is_power_of_two(y):
        reason = f"Non-Power of Two ({x}x{y})"
        recommendation = "Resize to nearest Power of Two for Mip-Maps"
        print(reason + "\n" + recommendation)
    
def check_mesh_collision(asset):
    asset_obj = asset.get_asset()
    body_setup = asset_obj.get_editor_property('body_setup')
    
    if not body_setup:
        print(f"CRITICAL: {asset.asset_name} has NO BodySetup!")
        return

    geom = body_setup.get_editor_property('agg_geom')
    
    total_shapes = (
        len(geom.get_editor_property('box_elems')) +
        len(geom.get_editor_property('sphere_elems')) +
        len(geom.get_editor_property('tapered_capsule_elems')) +
        len(geom.get_editor_property('convex_elems'))
    )

    if total_shapes == 0:
        print(f"{asset.asset_name} has NO Simple Collision shapes.")