
# Comprehensive Trie Data Structure Implementation

## Overview

This repository contains a robust and feature-rich C++ implementation of the Trie data structure, optimized for efficient string retrieval and manipulation. Tries are fundamental data structures used in various applications such as autocomplete systems, spell checkers, and IP routing. This project aims to provide a solid foundation for any application that could benefit from a Trie's capabilities.

## Features

- **Insert**: Add words to the Trie with time complexity O(length of word).
- **Search**: Efficiently check if a word exists in the Trie.
- **Delete**: Remove words from the Trie, ensuring optimal memory usage.
- **Weight Decay**: Apply decay to node weights, useful for prioritizing recent or frequent entries.
- **Optimize Paths**: Dynamically optimize search paths based on usage patterns.
- **View Trie**: Display a visual representation of the Trie's structure for debugging and analysis.
- **Prefix Search**: Retrieve all words sharing a common prefix, ideal for autocomplete functionalities.
- **Wildcard Search**: Support for wildcard searches, enhancing the Trie's flexibility in pattern matching.
- **Auto-Complete**: Generate auto-complete suggestions based on a given prefix.

## Getting Started

### Prerequisites

- A modern C++ compiler capable of C++11 (e.g., GCC, Clang)
- Basic knowledge of C++ and data structures

### Installation

1. **Clone the repository:**
    ```bash
    git clone https://github.com/123456Arshia/DWTT.git
    ```
2. **Navigate to the project directory:**
    ```bash
    cd DWTT
    ```

### Compilation

Compile the project using a C++11 compatible compiler:

```bash
g++ -std=c++11 -o TrieApp main.cpp Trie.cpp TrieNode.cpp Cli.cpp
```

### Running the Application

Execute the compiled application:

```bash
./TrieApp
```

Follow the on-screen prompts to interact with the Trie.

## Usage

The application provides a command-line interface (CLI) with a range of operations. Here's a brief on how to use each:

- **Insert**: Enter the word you wish to add to the Trie.
- **Search**: Input the word you want to find.
- **Delete**: Specify the word you intend to remove from the Trie.
- **View Trie**: No input needed; it visually represents the Trie's current state.
- **Prefix/Wildcard Search/Auto-Complete**: Provide the relevant prefix or pattern.

## Documentation

Detailed documentation for each method and its usage can be found in the respective header files. Comments and explanations are provided throughout the code to guide you through the implementation.

## Contributing

Contributions are what make the open-source community such a fantastic place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Authors & Acknowledgment

- [Your Name] - *Initial work* - [YourGithubProfile]
- Thanks to all the contributors who have helped shape this project.

## Contact

Arshia Taghavinejad -  arshiataghavinehjad@gmail.com

Project Link: [https://github.com/your_username/repo_name](https://github.com/123456Arshia/DWTT)

## Additional Notes

- This project is a work in progress and subject to significant changes.
- Feedback and suggestions are welcome and can be submitted via GitHub issues.

---
