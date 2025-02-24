package com.example;  

import org.apache.lucene.analysis.standard.StandardAnalyzer;  
import org.apache.lucene.document.Document;  
import org.apache.lucene.document.Field;  
import org.apache.lucene.document.TextField;  
import org.apache.lucene.index.IndexWriter;  
import org.apache.lucene.index.IndexWriterConfig;  
import org.apache.lucene.store.Directory;  
import org.apache.lucene.store.RAMDirectory;  

import org.apache.tika.exception.TikaException;  
import org.apache.tika.metadata.Metadata;  
import org.apache.tika.parser.ParseContext;  
import org.apache.tika.parser.pdf.PDFParser;  
// import org.apache.tika.parser.wordperfect.*;  
import org.apache.tika.sax.BodyContentHandler;  

import org.xml.sax.SAXException;  
import org.apache.poi.hwpf.HWPFDocument;
import org.apache.poi.hwpf.extractor.WordExtractor;
import org.apache.poi.xwpf.usermodel.XWPFDocument;
import org.apache.poi.xwpf.extractor.XWPFWordExtractor;
import org.jsoup.Jsoup;
// import org.jsoup.nodes.Document;
// import org.jsoup.nodes.File;

import java.io.FileInputStream;  
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.File;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class DocumentIndexer {  
    private final IndexWriter writer;  
    private final Directory directory;  

    public DocumentIndexer() throws IOException {  
        directory = new RAMDirectory();  
        IndexWriterConfig config = new IndexWriterConfig(new StandardAnalyzer());  
        writer = new IndexWriter(directory, config);  
    }  

    /*
     * Index a document at the given path and name.
     * 
     * @param filePath the path to the document to index
     * @param fileName the name of the document to index
     * @throws IOException if there is an error reading or writing to the index
     * @throws SAXException if there is an error parsing the document
     * @throws TikaException if there is an error parsing the document with Tika
     */
    public void indexDocument(String filePath, String fileName) throws IOException, SAXException, TikaException {  
        BodyContentHandler handler = new BodyContentHandler();  
        Metadata metadata = new Metadata();  
        FileInputStream inputStream = new FileInputStream(filePath); 
        String fileContent = "";

        if (filePath.endsWith(".pdf")) {  
            PDFParser pdfParser = new PDFParser();  
            pdfParser.parse(inputStream, handler, metadata, new ParseContext()); 
            fileContent = handler.toString();
        } else if (filePath.endsWith(".doc")) {
            HWPFDocument doc = new HWPFDocument(inputStream);
            WordExtractor extractor = new WordExtractor(doc);
            fileContent = extractor.getText();
            extractor.close();
        } else if (filePath.endsWith(".docx")) {
            XWPFDocument docx = new XWPFDocument(inputStream);
            XWPFWordExtractor extractor = new XWPFWordExtractor(docx);
            fileContent = extractor.getText();
            extractor.close();
        } else if (filePath.endsWith(".html")) {
            org.jsoup.nodes.Document jsoupDoc = Jsoup.parse(new File(filePath), "UTF-8");
            fileContent = jsoupDoc.body().text();
        }
        // } else if (filePath.endsWith(".txt")) {
        //     StringBuilder sb = new StringBuilder();
        //     try (BufferedReader br = new BufferedReader(new InputStreamReader(inputStream))) {
        //         String line;
        //         while ((line = br.readLine())!= null) {
        //             sb.append(line).append("\n");
        //         }
        //     }
        //     fileContent = sb.toString();
        // }

        Document doc = new Document();  
        doc.add(new TextField("content", fileContent, Field.Store.YES));  
        doc.add(new TextField("title", fileName, Field.Store.YES));  
        writer.addDocument(doc);  

        inputStream.close();  
    }  

    /*
     * Close the index writer and release any resources.
     * 
     * @throws IOException if there is an error closing the index writer
     */
    public void close() throws IOException {  
        writer.close();  
    }  

    /*
     * Get the directory containing the index.
     * 
     * @return the directory containing the index
     */
    public Directory getDirectory() {  
        return directory;  
    }  
}