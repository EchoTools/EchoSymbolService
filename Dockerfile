# Use an official image as a base image
FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive

# Set the working directory inside the container
WORKDIR /app

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Copy the C++ project files into the container
COPY . .

# Create a build directory
RUN mkdir build

# Set the working directory to the build directory
WORKDIR /app/build

# Run CMake to generate build files
RUN cmake -G Ninja ..

# Build the C++ project using Ninja
RUN ninja

# Set the working directory back to the project directory
WORKDIR /app

# Specify the command to run when the container starts
CMD ["./build/EchoSymbolService"]
