/*
 * Copyright (c) 2024. <jyt555>
 * All Rights Reserved.
 * This is a simple search engine application that indexes and searches documents.
 */
package com.example;  

import org.apache.tika.exception.TikaException;  
import org.xml.sax.SAXException;
import java.io.File;  
import java.io.IOException;  
import java.util.Scanner;  

public class SearchEngineApp {  
    public static void main(String[] args) {  
        try {  
            DocumentIndexer indexer = new DocumentIndexer();  
            Scanner scanner = new Scanner(System.in);  

            // �����ĵ�  
            System.out.println("\33[33m������Ҫ�������ļ���·��('exit'�˳���'enter'ʹ��Ĭ��·��resource):\33[0m");  
            String folderPath;  
            while (!(folderPath = scanner.nextLine()).equals("exit")) {  
                if (folderPath.trim().isEmpty())
                    folderPath = "src\\main\\resource"; // Ĭ��·��  
                File folder = new File(folderPath);  
                if (folder.exists() && folder.isDirectory()) {  
                    File[] files = folder.listFiles();  
                    if (files != null) {  
                        for (File file : files) {  
                            if (file.isFile()) {  // ȷ�����ļ�  
                                try {  
                                    indexer.indexDocument(file.getAbsolutePath(), file.getName());  
                                    System.out.println("\33[32m�ĵ�������: " + file.getAbsolutePath() + "\33[0m");  
                                } catch (SAXException e) {  
                                    System.out.println("\33[31m�ĵ�����ʧ��: " + file.getAbsolutePath() + "\33[0m");  
                                    e.printStackTrace();  
                                }  
                            }  
                        }  
                    }   
                } else {  
                    System.out.println("\33[31mָ����·�������ڻ���һ���ļ��С����������룺 \33[0m");  
                }  
                System.out.println("\33[33m������Ҫ�������ļ���·��('exit'�˳���'enter'ʹ��Ĭ��·��resource):\33[0m");
            }  

            indexer.close();  
            System.out.println("\33[32m������ɡ�\33[0m");  

            // �����ĵ�  
            SearchEngine searchEngine = new SearchEngine(indexer.getDirectory());  
            System.out.println("\33[33m�����������ؼ��ʣ����� 'exit' �˳�����\33[0m");  
            String searchQuery;  
            while (!(searchQuery = scanner.nextLine()).equals("exit")) {  
                SearchResult result = searchEngine.search(searchQuery);  
                result.displayResults(searchQuery);  
                System.out.println("\33[33m�����������ؼ��ʣ����� 'exit' �˳�����\33[0m");  
            }  

            scanner.close();  
        } catch (IOException | TikaException | org.apache.lucene.queryparser.classic.ParseException e) {  
            e.printStackTrace();  
        }  
    }  
}