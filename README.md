# WhyNow

**An Intent-Activity Alignment and Digital Behaviour Awareness System**

WhyNow is a command-line based digital behaviour awareness application written in C. It helps users compare what they intended to do during a digital session with the activities they actually performed.

Rather than simply measuring screen time, WhyNow focuses on the relationship between **intent and activity**. It records sessions, tracks activities, identifies behavioural patterns, preserves session history, and generates historical reports to help users understand their digital behaviour.

---

## Project Overview

Digital activity is often measured only in terms of time spent on a device. However, the amount of time spent does not explain whether that activity was aligned with the user's original purpose.

WhyNow addresses this problem by allowing a user to:

1. Define the purpose of a digital session.
2. Record activities performed during that session.
3. Classify activities according to their relationship with the original intent.
4. Measure intent alignment.
5. Record distraction reasons.
6. Save completed sessions persistently.
7. Review previous session history.
8. Generate behaviour insights.
9. Generate historical reports across multiple sessions.

The system is designed for **awareness rather than judgment**. It reports behavioural information without labelling users as productive or unproductive.

---

## Core Features

### Session Management

WhyNow allows users to create and manage digital sessions.

A session records:

- Session ID
- Original and current intent
- Device type
- Goal
- Expected duration
- Start and end time
- Actual duration
- Activities
- Intent alignment
- Goal status
- Satisfaction
- Reflection

Session IDs continue from previously stored sessions when the application is restarted.

### Activity Tracking

Activities performed during an active session can be recorded with:

- Activity ID and name
- Activity category
- Start and end time
- Duration
- Classification
- Distraction reason
- Context

Activities are classified as:

- Aligned
- Contextual
- Intent Change
- Drift
- No Purpose

### Behaviour Insights

WhyNow can analyse an individual session and calculate:

- Total activities
- Aligned activities
- Contextual activities
- Intent changes
- Drift activities
- Classification percentages
- Most common distraction reason
- Dominant behaviour pattern

### Persistent Session Storage

Completed sessions are stored persistently so that session information remains available after the application exits.

The storage module supports:

- Storage initialization
- Storage file detection and creation
- Session saving
- Session loading
- Capacity handling
- Invalid argument handling
- Human-readable storage status messages

Runtime session data is kept separate from tracked source code.

### Session History

Previously completed sessions can be viewed from the command-line interface.

Stored history includes:

- Session ID
- Intent
- Device
- Goal
- Activity count
- Intent alignment
- Session duration

### Historical Behaviour Reporting

WhyNow aggregates persisted sessions into a historical behaviour report containing:

- Total sessions and activities
- Total tracked time
- Average session duration
- Aligned, contextual, intent-change and drift totals
- Average, highest and lowest intent alignment
- Sessions with goals
- Completed goals
- Goal completion percentage
- Most common intent
- Most common distraction reason
- Dominant historical behaviour pattern

This provides long-term behavioural awareness across multiple sessions.

---

## Application Menu

The command-line interface currently provides:

```text
1. Start New Session
2. View Active Session
3. Record Activity
4. End Current Session
5. View Session History
6. View Behaviour Insights
7. View Historical Report
8. About & Privacy
0. Exit
```

---

## Project Architecture

WhyNow follows a modular C architecture. Each major responsibility is separated into its own source and header files.

```text
                 +------------------------+
                 |          User          |
                 +-----------+------------+
                             |
                             v
                 +------------------------+
                 | Command-Line Interface |
                 +-----------+------------+
                             |
          +------------------+------------------+
          |                                     |
          v                                     v
 +----------------+                    +----------------+
 | Session Module |                    | Activity Module|
 +-------+--------+                    +-------+--------+
         |                                     |
         +------------------+------------------+
                            |
                            v
                  +--------------------+
                  | Behaviour Insights |
                  +---------+----------+
                            |
                            v
                  +--------------------+
                  |   Storage Module   |
                  +---------+----------+
                            |
                            v
                  +--------------------+
                  | Persistent History |
                  +---------+----------+
                            |
                            v
                  +--------------------+
                  | Historical Reports |
                  +--------------------+
```

---

## Project Structure

```text
WhyNow/
|
|-- .github/
|   `-- workflows/
|       `-- ci.yml
|
|-- data/
|   `-- .gitkeep
|
|-- include/
|   |-- activity.h
|   |-- insights.h
|   |-- report.h
|   |-- session.h
|   |-- storage.h
|   `-- whynow.h
|
|-- src/
|   |-- activity.c
|   |-- insights.c
|   |-- main.c
|   |-- report.c
|   |-- session.c
|   `-- storage.c
|
|-- tests/
|   |-- run_tests.ps1
|   |-- run_tests.sh
|   |-- test_activity.c
|   |-- test_insights.c
|   |-- test_integration.c
|   |-- test_report.c
|   |-- test_session.c
|   `-- test_storage.c
|
|-- .gitignore
`-- README.md
```

---

## Module Overview

| Module | Responsibility |
|---|---|
| `session` | Session initialization, lifecycle and intent alignment |
| `activity` | Activity tracking, classification and context |
| `insights` | Behaviour analysis for individual sessions |
| `storage` | Persistent saving and loading of sessions |
| `report` | Historical analytics across stored sessions |
| `main` | Command-line interface and module integration |

---

## Building the Application

### Requirements

- GCC
- Git
- Bash environment for the Linux test runner
- PowerShell for the Windows test runner

### Linux / WSL

Compile:

```bash
gcc -Wall -Wextra -Wpedantic -Werror \
-Iinclude \
src/main.c \
src/session.c \
src/activity.c \
src/insights.c \
src/storage.c \
src/report.c \
-o whynow
```

Run:

```bash
./whynow
```

### Windows PowerShell

Compile:

```powershell
gcc -Wall -Wextra -Wpedantic -Werror `
-Iinclude `
src/main.c `
src/session.c `
src/activity.c `
src/insights.c `
src/storage.c `
src/report.c `
-o whynow.exe
```

Run:

```powershell
.\whynow.exe
```

---

## Automated Testing

WhyNow includes automated tests for all major modules.

### Linux / WSL

```bash
chmod +x tests/run_tests.sh
./tests/run_tests.sh
```

### Windows PowerShell

```powershell
powershell -ExecutionPolicy Bypass -File .\tests\run_tests.ps1
```

### Current Test Results

| Test Suite | Assertions | Status |
|---|---:|---|
| Session | 15 | PASS |
| Activity | 25 | PASS |
| Integration | 24 | PASS |
| Behaviour Insights | 20 | PASS |
| Storage | 19 | PASS |
| Historical Reporting | 28 | PASS |
| **Total** | **131** | **PASS** |

Current automated validation result:

```text
ALL TEST SUITES PASSED
Total assertions: 131
```

Application and test builds use strict compiler flags:

```text
-Wall -Wextra -Wpedantic -Werror
```

---

## Continuous Integration

WhyNow uses **GitHub Actions** for Continuous Integration.

Workflow file:

```text
.github/workflows/ci.yml
```

The CI pipeline runs for:

- Pushes to `main`
- Pushes to `feature/**` branches
- Pull requests targeting `main`

Pipeline:

```text
Repository Checkout
        |
        v
Strict Application Build
        |
        v
Automated Test Suites
        |
        v
   Pass / Fail
```

---

## Git and Collaboration Workflow

Development follows a feature-branch and pull-request based workflow.

```text
main
 |
 v
Feature Branch
 |
 v
Development
 |
 v
Local Build and Tests
 |
 v
Commit
 |
 v
Push
 |
 v
GitHub Actions CI
 |
 v
Pull Request
 |
 v
Code Review
 |
 v
Approval
 |
 v
Merge into main
```

Development has included separate work for:

- Session foundation
- Activity tracking and integration
- Automated and integration testing
- Behaviour insights
- Persistent session storage
- Historical reporting

---

## Data and Privacy

WhyNow records information explicitly entered by the user for digital behaviour awareness.

The application does not attempt to secretly monitor device activity.

User-provided information may include:

- Intent
- Goal
- Device type
- Activities
- Activity context
- Distraction reasons
- Session reflection

Persistent runtime data is kept separate from tracked source code.

WhyNow is designed to provide **non-judgmental behavioural awareness** rather than productivity scoring.

---

## Team

| Team Member | Contribution Areas |
|---|---|
| **Amar Kumar** | Core integration, behaviour insights, historical reporting, testing and project integration |
| **Sarvesh Kumar** | Persistent session storage, session history and storage validation |
| **Rohan Yadav** | Integration testing, CI workflow, automated validation and code review |

The project has been developed collaboratively using Git branches, commits, pull requests, automated CI checks and code reviews.

---

## Current Project Status

The major functional modules of WhyNow are implemented and integrated.

```text
Session Management             PASS
Activity Tracking              PASS
Behaviour Insights             PASS
Persistent Storage             PASS
Session History                PASS
Historical Reporting           PASS
Integration Testing            PASS
Strict Application Build       PASS
GitHub Actions CI              PASS
Automated Assertions           131/131 PASS
```

The project is currently in the **documentation, repository polish and final release preparation phase**.

---

## Future Improvements

Possible future extensions include:

- Improved interactive CLI experience
- Configurable storage capacity
- Date-based historical report filtering
- More detailed behavioural trend analysis
- Exportable reports
- Data visualization
- Additional privacy controls
- Configurable user preferences
- Additional integration and end-to-end tests
- Graphical or web-based interface

---

## Purpose

WhyNow demonstrates how software engineering and DevOps practices can be combined with a practical digital behaviour awareness application.

The project demonstrates:

- Modular C programming
- Automated testing
- Integration testing
- Persistent storage
- Behaviour analytics
- Historical reporting
- Git-based collaboration
- Feature branch development
- Pull request reviews
- Continuous Integration with GitHub Actions
- Cross-platform validation

---

**WhyNow — understand not just how long you were online, but whether your activity matched why you went online.**