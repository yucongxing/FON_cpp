# Focus On Now (FON)

A desktop focus-timer application with real-time camera-based attention detection. Set a task, start the timer, and the app monitors whether you stay focused using face and eye detection.

## Features

- **Focus timer** — Set task name and duration (MM:SS), with start / pause / stop controls and a live LCD countdown
- **Live camera feed** — Raw camera output displayed during a focus session
- **Attention detection** — Haar cascade face and eye detection running on a dedicated thread; annotated output shown alongside the raw feed
- **Focus score** — Real-time 0–100 score based on a 60-second rolling window, displayed as an overlay on the annotated frame
- **Login screen** — Account system placeholder, ready to connect to a backend

## Architecture

```
main()
 └── LoginDialog ──emit loginSuccess()──► main() creates MainWindow
                                                │
                               ┌────────────────┴────────────────┐
                          LeftWidget                         CameraWorker (QThread)
                       (task timer UI)                            │
                     start/stop signals                  ┌────────┴────────┐
                           │                        FocusAnalyzer     FocusScorer
                           └──► startCapture()      (Haar cascade)  (state machine)
                                stopCapture()            │                │
                                                  frameReady()    focusScoreUpdated()
                                                         │                │
                                                  show_frame1      score_overlay
                                                  show_frame2     (top-left of frame2)
```

**Data flow per frame (worker thread):**
1. `CameraCapture` reads a frame from the webcam
2. Every 3rd frame: `FocusAnalyzer` runs face detection → eye detection per face ROI
3. `FocusAnalyzer::drawResults` draws green rectangle (face) and blue circles (eyes) onto a clone of the frame
4. `FocusScorer::update` applies 300 ms debounce state machine → `FOCUSED` / `DISTRACTED`
5. Signals cross to the UI thread: `frameReady(original, analyzed)` and `focusScoreUpdated(int)` every second

## Module Structure

```
src/
├── main.cpp
├── algorithm/              # Pure C++, no Qt dependency
│   ├── CameraCapture       # OpenCV VideoCapture wrapper
│   ├── FocusAnalyzer       # Haar face + eye detection, frame annotation
│   └── FocusScorer         # Focus state machine + rolling-window score
└── UI/                     # Qt5 widgets
    ├── LoginDialog         # Login screen, emits loginSuccess()
    ├── MainWindow          # Main window, owns CameraWorker
    ├── LeftWidget          # Task timer sidebar
    └── CameraWorker        # QThread: capture loop + algorithm bridge
```

## Dependencies

| Dependency | Version | Purpose |
|---|---|---|
| CMake | ≥ 3.25 | Build system |
| C++ | 17 | Language standard |
| Qt5 | any | GUI (Core, Gui, Widgets) |
| OpenCV | any | Camera capture, Haar detection (core, videoio, imgproc, objdetect) |

## Build

```bash
# Configure (MinGW example)
cmake -B build -G "MinGW Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=/path/to/gcc.exe \
  -DCMAKE_CXX_COMPILER=/path/to/g++.exe \
  -DCMAKE_MAKE_PROGRAM=/path/to/mingw32-make.exe \
  -DQt5_DIR=/path/to/Qt/5.15.2/mingw81_64/lib/cmake/Qt5 \
  -DOpenCV_DIR=/path/to/opencv/build

# Build
cmake --build build

# Run
./build/src/FON
```

CMake will automatically copy the required Haar cascade files from the OpenCV installation to `<build>/data/haarcascades/` after the build. If detection is disabled on first run, check the console for a `[CameraWorker]` warning indicating the cascade files were not found.

> **Manual fallback:** If CMake cannot locate the cascade files automatically, copy the following from your OpenCV installation to the executable directory:
> ```
> <executable>/data/haarcascades/haarcascade_frontalface_default.xml
> <executable>/data/haarcascades/haarcascade_eye.xml
> ```

## Usage

1. Launch the application — a login dialog appears
2. Enter credentials and click **Login** (server integration pending)
3. Enter a task name and duration in the left sidebar
4. Click **开始** — the camera starts, the countdown begins
5. The left frame shows the raw camera feed; the right frame shows detection overlays
6. The **专注度 N%** badge in the top-left corner of the right frame updates every second
7. Click **暂停** to pause or **停止** to end the session — the camera stops immediately

## Focus Score Details

| Parameter | Value |
|---|---|
| Unfocus debounce | 300 ms |
| Score rolling window | 60 seconds |
| Detection frequency | Every 3rd frame |
| "Focused" condition | Face detected AND at least one eye detected |

The score is the percentage of the last 60 seconds spent in the `FOCUSED` state. Natural blinks (< 300 ms) do not trigger a state change.

## Roadmap

- [ ] HTTP login — connect `LoginDialog` to account management server
- [ ] Session report — display focused time, distraction count, and longest streak after session ends
- [ ] Visual LLM integration — periodic frame analysis for higher-level attention assessment
- [ ] Eye landmark / gaze estimation for more accurate focus detection
