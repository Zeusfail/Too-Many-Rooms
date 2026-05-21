# Too Many Rooms Free Building

> 🚧 **EXPERIMENTAL RELEASE READ BEFORE INSTALLING** 🚧

---

## ⚠️ WARNING

This mod has only been tested by myself on **Microsoft Store / Game Pass** and **Steam**.  
It has **NOT** gone through extended testing or a full release protocol as I usually do.  
Crashes, bugs, and unexpected behavior are possible.  
**USE AT YOUR OWN RISK** and please report your experience in the comments!

### 🙋 TESTERS WANTED
Your feedback is essential to make this stable. Every report helps. 🙏

---

## Description

**TooManyRooms** removes base placement collision restrictions in Subnautica 2, allowing you to build your habitat freely disabling the overdone obstruction boundaries when placing and resizing rooms.

### ✅ What this mod does:
- Removes collision restrictions when placing base rooms and extensions
- Lets you build in areas that would normally be flagged as invalid

### ❌ What this mod does NOT do (yet):
- Free placement **inside** the base (building furniture/items inside rooms) this uses a different system and is not affected *(will come later)*

---

## How it works

This is a C++ DLL mod loaded by UE4SS. It patches two memory locations at runtime to bypass the placement validation checks. **No game files are modified** the patch is applied in memory only and disappears when you close the game.

---

## ⚠️ Common Installation Mistakes

Please read before posting a bug report!

- UE4SS and `dwmapi.dll` must be extracted **directly into `WinGDK\`** not inside a subfolder
- `TooManyRooms` must be placed **directly inside `Mods\`** not nested inside another folder
- When unzipping, make sure to move `TooManyRooms\` out of any parent folder created by your unzip tool
- `enabled.txt` must be present inside the `TooManyRooms` folder

---

## Installation

1. [Install UE4SS for Subnautica 2](https://github.com/UE4SS-RE/RE-UE4SS)
2. Locate your `Mods` folder:
   - **Microsoft Store / Game Pass:**  
     `Subnautica2\Binaries\WinGDK\ue4ss\Mods\`
3. Extract the ZIP and place the `TooManyRooms` folder directly into `Mods\`
4. Launch the game placement restrictions are now removed

**Expected folder structure:**
```
ue4ss + dwmapi.dll /
└── Mods/
    └── TooManyRooms/
        ├── enabled.txt
        ├── Scripts/
        │   └── main.lua
        └── dlls/
            └── main.dll
```

---

## Requirements

- [UE4SS for Subnautica 2](https://github.com/UE4SS-RE/RE-UE4SS)
- Subnautica 2 Early Access

---

## ❓ FAQ

**Q: Will this corrupt my save?**  
A: The patch is applied in memory only no game files are modified. Removing the mod folder is enough to fully uninstall.

**Q: Can I remove the mod mid-playthrough?**  
A: Yes delete the `TooManyRooms` folder. No files are modified.

**Q: The mod doesn't seem to work?**  
A: Almost always a folder structure issue check the *Common Installation Mistakes* section above. Also make sure `enabled.txt` is present.

**Q: Does the mod send any data externally?**  
A: No. No telemetry, no external calls. Fully local and open source.

**Q: Does it affect building furniture inside the base?**  
A: No inside-base building uses a separate system. Only external base placement is affected for now.

**Q: Is this mod dangerous?**  
A: Only for your base's structural integrity. 🪸

---

## 🔧 Known Limitations

- Only tested on **Microsoft Store / Game Pass** Steam compatibility unknown
- No extended multi-session testing behavior over long playthroughs is unknown
- May break with game updates offsets are version-specific
- Inside-base placement (furniture, equipment) is not affected

---

## Source Code

Available on [GitHub](https://github.com/zeusfail/Too-Many-Rooms) if you enjoy the mod :3

---

*Made with love from French Polynesia 🌺*  
💬 Discord **zeusfail**
