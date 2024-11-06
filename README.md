<h1 align="center">ISOMake</h1>
<p align="center">
  <b>ISO 9660 Image Creation Utility</b>
  <br><br>
  <img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/voltageddebunked/isomake?style=social">
  <img alt="License" src="https://img.shields.io/github/license/voltageddebunked/isomake?color=blue">
  <img alt="Last Commit" src="https://img.shields.io/github/last-commit/voltageddebunked/isomake">
  <img alt="Build" src="https://img.shields.io/badge/build-passing-brightgreen">
</p>

<p align="center">
ISOMake is a fast, flexible utility for creating ISO 9660 images. This command-line tool by VoltagedDebunked allows users to generate ISO files with ease, supporting various input files and directories.
</p>

---

<h2>🔧 Features</h2>

- Supports ISO 9660 format for compatibility across platforms
- Simple CLI with optional color-coded output
- Handles multiple file inputs for flexible image creation
- Detailed progress reporting for each stage of ISO creation

<h2>📦 Installation</h2>

```bash
git clone https://github.com/voltageddebunked/isomake.git
cd isomake
make install
```

<h2>🚀 Usage</h2>

To create an ISO image from one or more files, use the following command:

```bash
isomake <output.iso> <input_file1> [input_file2] ...
```

**Example:**

```bash
isomake output.iso file1.txt file2.jpg
```

This creates an ISO file (`output.iso`) containing `file1.txt` and `file2.jpg`.

<h2>🤝 Contributing</h2>

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/NewFeature`)
3. Commit your changes (`git commit -m 'Add NewFeature'`)
4. Push to the branch (`git push origin feature/NewFeature`)
5. Open a pull request

<h2>📝 License</h2>

ISOMake is licensed under the GPL-3.0 License. See the <a href="https://github.com/voltageddebunked/isomake/blob/main/LICENSE">LICENSE</a> file for details.

---

<p align="center">
<sub>Created by <a href="https://github.com/voltageddebunked">VoltagedDebunked</a></sub>
</p>