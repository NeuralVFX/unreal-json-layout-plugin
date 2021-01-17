![](Images/jenga_a.gif)
# Unreal-Json-Layout-Plugin

This plugin provides a GUI to manage the saving and loading of scene layouts.

## About

- Layouts are stored on disk as Json files
- All loading and unloading classes are derived from an abstract class
- Children of this abstract class handle loading/unloading of each type of asset in different ways
- This can be expanded to include things like Cameras or Skeletal Mesh with ease
- Currently this is demonstrating saving and loadig from inside Unreal, however this methodology works for transferring information between DCCs

## Plugin Usage
Usage instructions found here: [user manual page](USAGE.md).
