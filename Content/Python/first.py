import unreal

editor_util = unreal.EditorLevelLibrary()

# Get selected actors in the viewport
selected_actors = editor_util.get_selected_level_actors()

for actor in selected_actors:
    unreal.log(f"Selected: {actor.get_name()}")
    