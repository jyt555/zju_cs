package com.example;  

import java.util.ArrayList;  
import java.util.List;  

public class SearchResult {  
    private final List<String> titles;  
    private final List<String> contents;  

    public SearchResult(int size) {  
        titles = new ArrayList<>(size);  
        contents = new ArrayList<>(size);  
    }  

    /*
     * Add a document to the search result.
     *  
     * @param title The title of the document.
     * @param content The content of the document.
     */
    public void addDocument(String title, String content) {  
        titles.add(title);  
        contents.add(content);  
    }  

    /*
     * Display the search results for a given query.
     *  
     * @param s The queried string. 
     */
    public void displayResults(String s) { 
        System.out.println("----------RESULT FOR: " + s + "------------------"); 
        for (int i = 0; i < titles.size(); i++) { 
            if (!contents.get(i).contains(s))
                break;
            if (i != 0) 
                System.out.println("---");  
            System.out.println("\33[35m" + "Title: " + titles.get(i) + "\33[0m"); 
            String content = contents.get(i);  
            String[] lines = content.split("\n");  
            for (String line : lines) {  
                if(line.contains(s))
                    System.out.println("\33[34m" + "|_Content: " + line + "\33[0m");  
            }  
            // System.out.println("\33[34m" + "Content: " + contents.get(i) + "\33[0m");  
        }  
        System.out.println("----------------------------");  
    }  
}