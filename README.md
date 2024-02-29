# Web Crawler

## Overview
Welcome to my Web Crawler Project! This is a high-performance, efficient web crawler built in C. The crawler is designed to navigate through web pages starting from a seed URL, recursively process embedded links, and extract data, all while managing depth and avoiding external domains.

## Features
- **Recursive Crawling**: Starts from a seed URL and navigates through embedded links up to a specified depth.
- **Efficient URL Management**: Utilizes advanced data structures like hash tables and bags for storing and processing URLs.
- **Multithreading**: Implements multithreading to enhance crawling efficiency by processing multiple URLs in parallel.
- **Integration with libcurl**: Uses libcurl for handling HTTP requests, ensuring accurate URL fetching and processing.
- **Robust Error Handling**: Features comprehensive error handling and memory management for stable performance.
- **Modular Design**: The crawler's architecture is modular, allowing for easy maintenance and scalability.

## Installation
1. **Prerequisites**: Ensure you have `libcurl4` and `libcurl4-gnutls-dev` installed on your system. You can install them using:
   ```bash
   sudo apt install libcurl4 libcurl4-gnutls-dev
   ```
2. **Clone the Repository**: Clone this repository to your local machine using:
   ```bash
   git clone https://github.com/rajatmaheshwari17/Web-Crawler.git
   ```
3. **Build**: Compile the project using a suitable C compiler, ensuring you link the `libcurl` library.
   ```bash
   gcc -o webcrawler [source_files] -lcurl
   ```

## Usage
Run the crawler using the following command syntax:
```bash
./webcrawler [seedURL] [pageDirectory] [maxDepth]
```
- `seedURL` is the starting URL for the crawler.
- `pageDirectory` is the directory where the crawled pages will be stored.
- `maxDepth` is an integer defining the depth of crawling (0-10).

## Contributing
Contributions to this project are welcome! Feel free to fork the repository and submit pull requests.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Author
**Rajat Maheshwari**

Feel free to contact me for any questions or collaboration ideas. Thank you for checking out this project!

---

*This README is a part of the Web Crawler Project by Rajat Maheshwari.*
