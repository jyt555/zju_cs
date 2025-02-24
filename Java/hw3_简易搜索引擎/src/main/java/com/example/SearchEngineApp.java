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

            // 索引文档  
            System.out.println("\33[33m请输入要索引的文件夹路径('exit'退出，'enter'使用默认路径resource):\33[0m");  
            String folderPath;  
            while (!(folderPath = scanner.nextLine()).equals("exit")) {  
                if (folderPath.trim().isEmpty())
                    folderPath = "src\\main\\resource"; // 默认路径  
                File folder = new File(folderPath);  
                if (folder.exists() && folder.isDirectory()) {  
                    File[] files = folder.listFiles();  
                    if (files != null) {  
                        for (File file : files) {  
                            if (file.isFile()) {  // 确保是文件  
                                try {  
                                    indexer.indexDocument(file.getAbsolutePath(), file.getName());  
                                    System.out.println("\33[32m文档已索引: " + file.getAbsolutePath() + "\33[0m");  
                                } catch (SAXException e) {  
                                    System.out.println("\33[31m文档解析失败: " + file.getAbsolutePath() + "\33[0m");  
                                    e.printStackTrace();  
                                }  
                            }  
                        }  
                    }   
                } else {  
                    System.out.println("\33[31m指定的路径不存在或不是一个文件夹。请重新输入： \33[0m");  
                }  
                System.out.println("\33[33m请输入要索引的文件夹路径('exit'退出，'enter'使用默认路径resource):\33[0m");
            }  

            indexer.close();  
            System.out.println("\33[32m索引完成。\33[0m");  

            // 搜索文档  
            SearchEngine searchEngine = new SearchEngine(indexer.getDirectory());  
            System.out.println("\33[33m请输入搜索关键词（输入 'exit' 退出）：\33[0m");  
            String searchQuery;  
            while (!(searchQuery = scanner.nextLine()).equals("exit")) {  
                SearchResult result = searchEngine.search(searchQuery);  
                result.displayResults(searchQuery);  
                System.out.println("\33[33m请输入搜索关键词（输入 'exit' 退出）：\33[0m");  
            }  

            scanner.close();  
        } catch (IOException | TikaException | org.apache.lucene.queryparser.classic.ParseException e) {  
            e.printStackTrace();  
        }  
    }  
}