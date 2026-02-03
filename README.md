# UE5 Tools Project

![Unreal Engine](https://img.shields.io/badge/unreal_engine-black?style=for-the-badge&logo=unrealengine&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)

**UE5ToolsProject** is a learning project where I explore how to build custom tools in Unreal. It combines C++ coding, custom UI design, and Python scripting to create the kind of automation tools used in professional game studios.
Architecture and core implementation based on the 'Unreal Engine 5 C++ Editor Tools' curriculum by Vince Petrelli. Extended with custom modifications and personal study notes.

This repository demonstrates how to build:
- **Editor Modules & Plugins**
- **Slate UI Widgets** (Custom Editor Tabs)
- **Python Automation Scripts**
- **Content Browser Context Menu Extensions**

---

## 📸 Preview

### Advanced Deletion Tab(WIP)
<img width="912" height="612" alt="Screenshot 2026-02-03 at 1 41 58 AM" src="https://github.com/user-attachments/assets/4cbcb988-e5a3-44d4-ad10-29ea23785712" />

> A custom Slate widget allowing batch selection and deletion of assets with checkbox support.

### Context Menu Extensions
<img width="460" height="647" alt="Screenshot 2026-02-03 at 1 40 28 AM" src="https://github.com/user-attachments/assets/108a629f-3b71-4c7f-8a52-a6a052904bbb" />

> Quickly delete unused assets or empty folders directly from the Content Browser.

---

## 🌟 Key Features

### 1. SuperManager Plugin
The core C++ plugin providing editor enhancements:
- **Batch Operations:** Delete Unused Assets, Delete Empty Folders, and Fix Redirectors.
- **Custom Editor Tab:** A "Advanced Deletion" tool built with **Slate** to list, filter, and delete specific assets safely.
- **Asset Actions:** Quick actions to duplicate assets, batch rename, or add prefixes (e.g., adding `SM_` to Static Meshes automatically).

### 2. Python Automation
A Python script (`SuperManager.py`) that acts as a quality assurance tool:
- **Naming Convention Checks:** Ensures assets follow prefixes (e.g., `T_` for Textures, `BP_` for Blueprints).
- **Asset Reporting:** Generates logs for assets with missing prefixes or other issues.
- **Empty Folders: **Identifies and reports empty folders for cleanup.
- - **Texture Optimization:** Automatically flags textures with non-Power-of-Two dimensions.

---

## 📂 Repository Layout

- **`Plugins/SuperManager`** — The main Editor Plugin.
  - `SuperManager.cpp`: Handles module startup, shutdown, and menu registration.
  - `SlateWidgets/AdvanceDeletionWidget.cpp`: The C++ logic for the custom Slate UI.
  - `AssetAction/QuickAssetAction.cpp`: Helper functions for batch renaming and duplication.
- **`Content/Python/SuperManager.py`** — Python script for reporting and validation.
- **`Source/MyProject`** — Sample game module entry point.
