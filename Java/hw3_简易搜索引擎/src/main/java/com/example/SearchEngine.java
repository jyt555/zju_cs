package com.example;  

import org.apache.lucene.analysis.standard.StandardAnalyzer;  
import org.apache.lucene.document.Document;  
import org.apache.lucene.index.DirectoryReader;  
import org.apache.lucene.queryparser.classic.ParseException;  
import org.apache.lucene.queryparser.classic.QueryParser;  
import org.apache.lucene.search.IndexSearcher;  
import org.apache.lucene.search.Query;  
import org.apache.lucene.search.ScoreDoc;  
import org.apache.lucene.store.Directory;  

import java.io.IOException;  

public class SearchEngine {  
    private final IndexSearcher searcher;  

    /*
     * Constructor that takes an index directory as input and creates an IndexSearcher object.
     *  
     * @param indexDirectory the directory containing the Lucene index.
     * @throws IOException if an error occurs while opening the index.
     */
    public SearchEngine(Directory indexDirectory) throws IOException {  
        DirectoryReader directoryReader = DirectoryReader.open(indexDirectory);  
        searcher = new IndexSearcher(directoryReader);  
    }  

    /*
     * Searches the index for documents that match the given search string.
     *  
     * @param searchString the search string to use for the query.
     * @return a SearchResult object containing the search results.
     * @throws IOException if an error occurs while searching the index.
     * @throws ParseException if the search string is not valid.
     */
    public SearchResult search(String searchString) throws IOException, ParseException {  
        QueryParser parser = new QueryParser("content", new StandardAnalyzer());  
        Query query = parser.parse(searchString);  
        ScoreDoc[] hits = searcher.search(query, 10).scoreDocs;  

        SearchResult result = new SearchResult(hits.length);  
        for (ScoreDoc hit : hits) {  
            Document document = searcher.doc(hit.doc);  
            result.addDocument(document.get("title"), document.get("content"));  
        }  

        return result;  
    }  
}