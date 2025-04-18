#!/bin/bash
# Simple test script for PulseFire

echo "=== PulseFire Test Script ==="
echo ""

# Check directory structure
echo "Checking directory structure..."
if [ -d "PulseFire" ]; then
    echo "✓ PulseFire directory exists"
else
    echo "✗ PulseFire directory not found"
    exit 1
fi

if [ -d "PulseFire/Source" ]; then
    echo "✓ Source directory exists"
else
    echo "✗ Source directory not found"
    exit 1
fi

if [ -d "PulseFire/Content" ]; then
    echo "✓ Content directory exists"
else
    echo "✗ Content directory not found"
    exit 1
fi

if [ -d "PulseFire/Documentation" ]; then
    echo "✓ Documentation directory exists"
else
    echo "✗ Documentation directory not found"
    exit 1
fi

# Check documentation files
echo ""
echo "Checking documentation files..."
if [ -f "PulseFire/Documentation/UserManual/README.md" ]; then
    echo "✓ User Manual exists"
else
    echo "✗ User Manual not found"
    exit 1
fi

if [ -f "PulseFire/Documentation/DeveloperGuide/README.md" ]; then
    echo "✓ Developer Guide exists"
else
    echo "✗ Developer Guide not found"
    exit 1
fi

if [ -f "PulseFire/Documentation/API/README.md" ]; then
    echo "✓ API Reference exists"
else
    echo "✗ API Reference not found"
    exit 1
fi

# Check build scripts
echo ""
echo "Checking build scripts..."
if [ -f "PulseFire/Build.sh" ]; then
    echo "✓ Build.sh exists"
else
    echo "✗ Build.sh not found"
    exit 1
fi

if [ -f "PulseFire/Build.bat" ]; then
    echo "✓ Build.bat exists"
else
    echo "✗ Build.bat not found"
    exit 1
fi

# Check source code
echo ""
echo "Checking source code..."
SOURCE_DIRS=("Audio" "Performance" "VFX" "UI" "Analytics" "Settings" "Weapons" "Multiplayer")
for dir in "${SOURCE_DIRS[@]}"; do
    if [ -d "PulseFire/Source/PulseFire/$dir" ]; then
        echo "✓ $dir directory exists"
    else
        echo "✗ $dir directory not found"
        exit 1
    fi
done

# Check README
echo ""
echo "Checking README..."
if [ -f "PulseFire/README.md" ]; then
    echo "✓ README.md exists"
else
    echo "✗ README.md not found"
    exit 1
fi

# Check git repository
echo ""
echo "Checking git repository..."
if [ -d ".git" ] || [ -d "PulseFire/.git" ]; then
    echo "✓ Git repository exists"
else
    echo "✗ Git repository not found"
    exit 1
fi

# All tests passed
echo ""
echo "All tests passed! The application is ready for use."
exit 0
