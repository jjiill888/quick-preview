
# quick-preview

`quick-preview` is a lightweight standalone file preview tool for Linux, similar to macOS's Quick Look. It is not a Thunar plugin and does not depend on any file manager. You can trigger it manually or bind it to the spacebar key using your window manager or keyboard shortcut tools.

## Features

- Preview text files and images
- Auto-scale large content
- Preview folder contents (up to 100 items)
- Display "Unsupported file type" for unknown formats
- Exit when clicking outside the window or pressing the spacebar
- Minimal design with no window decoration

## Dependencies

- GTK+ 3
- GDK-Pixbuf
- (Optional) Poppler-glib (for future PDF support)

Install them on Debian/Ubuntu-based systems:

```bash
sudo apt install libgtk-3-dev libgdk-pixbuf2.0-dev
```

## Build

```bash
gcc quick-preview.c -o quick-preview $(pkg-config --cflags --libs gtk+-3.0 gdk-pixbuf-2.0) -lm
```

## Usage

```bash
./quick-preview <file_or_directory>
```

## Keyboard Binding (XFCE Example)

1. Go to *Settings* → *Keyboard* → *Application Shortcuts*
2. Add a new shortcut:

```bash
quick-preview %f
```

3. Bind it to the spacebar (or another key)

Make sure `%f` passes the currently selected file path.

## Roadmap

- PDF preview (Poppler)
- Carousel for browsing sibling files
- Archive preview
- Media playback

## License

MIT
