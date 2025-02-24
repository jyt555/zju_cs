# Search Engine  

This project implements a simple search engine using Java with Jsoup, Apache Tika, and Lucene.  

## Project Structure  
.
├─src
│  └─main
│      ├─added: Directory to store documents added for testing.  
│      ├─java: Contains the Java source code.  
│      └─resource: (Default) Directory to store documents for indexing.  
|
|─target: Directory to store compiled classes and other build artifacts.  
|
└─pom.xml: Maven project file.  

## Dependencies  
- Apache Tika for parsing documents(.pdf, .docx, .doc).  
- Jsoup for HTML parsing.  
- Apache Lucene for indexing and searching.  

## Build with Maven  
To build the project, run:  
```bash  
mvn clean install
```

## Run the program
```bash  
mvn exec:java -Dexec.mainClass="com.example.SearchEngineApp"
```