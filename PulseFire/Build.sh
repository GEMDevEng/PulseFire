#!/bin/bash
# PulseFire Build Script
# This script automates the build process for PulseFire

# Set default values
PLATFORM="Win64"
CONFIGURATION="Development"
ARCHIVE=false
CLEAN=false
DOCS=false
TESTS=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  key="$1"
  case $key in
    -p|--platform)
      PLATFORM="$2"
      shift
      shift
      ;;
    -c|--configuration)
      CONFIGURATION="$2"
      shift
      shift
      ;;
    -a|--archive)
      ARCHIVE=true
      shift
      ;;
    --clean)
      CLEAN=true
      shift
      ;;
    --docs)
      DOCS=true
      shift
      ;;
    --tests)
      TESTS=true
      shift
      ;;
    -h|--help)
      echo "PulseFire Build Script"
      echo "Usage: ./Build.sh [options]"
      echo ""
      echo "Options:"
      echo "  -p, --platform PLATFORM      Target platform (Win64, Mac, Linux)"
      echo "  -c, --configuration CONFIG   Build configuration (Development, Shipping, Debug)"
      echo "  -a, --archive                Create a distributable archive"
      echo "      --clean                  Clean build artifacts before building"
      echo "      --docs                   Generate documentation"
      echo "      --tests                  Run tests"
      echo "  -h, --help                   Show this help message"
      exit 0
      ;;
    *)
      echo "Unknown option: $key"
      echo "Use --help for usage information"
      exit 1
      ;;
  esac
done

# Set UE4 path based on platform
if [[ "$OSTYPE" == "darwin"* ]]; then
  UE_PATH="/Applications/Epic Games/UE_5.4/Engine/Build/BatchFiles/Mac/Build.sh"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  UE_PATH="/opt/unreal-engine/Engine/Build/BatchFiles/Linux/Build.sh"
else
  UE_PATH="C:/Program Files/Epic Games/UE_5.4/Engine/Build/BatchFiles/Build.bat"
fi

# Set output directory
OUTPUT_DIR="Build/$PLATFORM/$CONFIGURATION"
mkdir -p "$OUTPUT_DIR"

# Clean build artifacts if requested
if [ "$CLEAN" = true ]; then
  echo "Cleaning build artifacts..."
  rm -rf Binaries
  rm -rf Intermediate
  rm -rf Build
  mkdir -p "$OUTPUT_DIR"
fi

# Build the project
echo "Building PulseFire for $PLATFORM ($CONFIGURATION)..."
if [[ "$OSTYPE" == "darwin"* || "$OSTYPE" == "linux-gnu"* ]]; then
  "$UE_PATH" PulseFireEditor "$PLATFORM" "$CONFIGURATION" -project="$(pwd)/PulseFire.uproject"
  "$UE_PATH" PulseFire "$PLATFORM" "$CONFIGURATION" -project="$(pwd)/PulseFire.uproject"
else
  "$UE_PATH" PulseFireEditor "$PLATFORM" "$CONFIGURATION" -project="$(pwd)/PulseFire.uproject"
  "$UE_PATH" PulseFire "$PLATFORM" "$CONFIGURATION" -project="$(pwd)/PulseFire.uproject"
fi

# Check if build was successful
if [ $? -ne 0 ]; then
  echo "Build failed!"
  exit 1
fi

# Run tests if requested
if [ "$TESTS" = true ]; then
  echo "Running tests..."
  # Add test command here
fi

# Generate documentation if requested
if [ "$DOCS" = true ]; then
  echo "Generating documentation..."
  # Add documentation generation command here
fi

# Create archive if requested
if [ "$ARCHIVE" = true ]; then
  echo "Creating archive..."
  ARCHIVE_NAME="PulseFire-$PLATFORM-$CONFIGURATION.zip"
  if [[ "$OSTYPE" == "darwin"* || "$OSTYPE" == "linux-gnu"* ]]; then
    zip -r "$OUTPUT_DIR/$ARCHIVE_NAME" Binaries Content
  else
    powershell Compress-Archive -Path Binaries,Content -DestinationPath "$OUTPUT_DIR/$ARCHIVE_NAME" -Force
  fi
  echo "Archive created at $OUTPUT_DIR/$ARCHIVE_NAME"
fi

echo "Build completed successfully!"
exit 0
