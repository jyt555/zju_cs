import entities.Book;
import entities.Borrow;
import entities.Card;
import queries.*;
import utils.ConnectConfig;
import utils.DatabaseConnector;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.logging.Logger;

public class Main {

    private static final Logger log = Logger.getLogger(Main.class.getName());

    public static void main(String[] args) {
        try {
            // parse connection config from "resources/application.yaml"
            ConnectConfig conf = new ConnectConfig();
            log.info("Success to parse connect config. " + conf.toString());
            // connect to database
            DatabaseConnector connector = new DatabaseConnector(conf);
            boolean connStatus = connector.connect();
            if (!connStatus) {
                log.severe("Failed to connect database.");
                System.exit(1);
            }
            /* do something */
            // Create an instance of LibraryManagementSystemImpl
            LibraryManagementSystemImpl libraryManagementSystem = new LibraryManagementSystemImpl(connector);
            System.out.println("---Welcome to the library---");
            System.out.println("Input the number to operate:");
            System.out.println(" [1] store book         [2] delta book stock        [3] batch store books");
            System.out.println(" [4] remove book        [5] modify book info        [6] query book");
            System.out.println(" [7] borrow book        [8] return book             [9] show borrow history");
            System.out.println("[10] register card     [11] remove card            [12] show cards");
            System.out.println("[13] reset database     [0] exit\n");

            int choice = -1;
            String nextline;
            String category, title, press, author;
            int bookID, publishYear, stock = 0;
            double price;
            Book book;
            int cardID = 0;
            String name, department, type_str;
            Card.CardType type = null;
            Card card;
            Borrow borrow;
            ApiResult res;
            do{
                System.out.println("Please enter your choice:");
                Scanner scanner = new Scanner(System.in);
                if (scanner.hasNextInt())
                    choice = scanner.nextInt();
                else{
                    System.out.println("Invalid input, please enter integer");
                    continue;
                }
                scanner.nextLine(); // !!!
                switch(choice){
                    case 0: // exit
                        break;
                    case 1: // store book
                        System.out.println("Please enter the book information:");
                        System.out.print("Category:");
                        category = scanner.nextLine();
                        System.out.print("Title:");
                        title = scanner.nextLine();
                        System.out.print("Press:");
                        press = scanner.nextLine();
                        System.out.print("PublishYear:");
                        publishYear = scanner.nextInt();
                        scanner.nextLine();
                        System.out.print("Author:");
                        author = scanner.nextLine();
                        System.out.print("Price:");
                        price = scanner.nextDouble();
                        scanner.nextLine();
                        System.out.print("Stock:");
                        stock = scanner.nextInt();
                        scanner.nextLine();
                        book = new Book(category, title, press, publishYear, author, price, stock);
                        res = libraryManagementSystem.storeBook(book);
                        if (res.ok)
                            System.out.println("Successfully store book, book_id:" + res.payload);
                        else
                            System.out.println(res.message);
                        break;
                    case 2: // delta book stock
                        int delta;
                        System.out.println("Please enter the book_id and the delta_number:");
                        System.out.print("Book_id:");
                        bookID = scanner.nextInt();
                        scanner.nextLine();
                        System.out.print("Delta_number:");
                        delta = scanner.nextInt();
                        scanner.nextLine();
                        res = libraryManagementSystem.incBookStock(bookID, delta);
                        if (res.ok)
                            System.out.println("Successfully delta book stock");
                        else
                            System.out.println(res.message);
                        break;
                    case 3: // batch store books
                        List<Book> books = new ArrayList<>();
                        System.out.println("Please enter the book information:");
                        System.out.println("Format(split by ','): category, title, press, publishYear, author, price, stock");
                        String input = scanner.nextLine(); // there are so many item, so they can't be loaded like case1
                        while (!input.isEmpty()) {
                            String[] bookInfo = input.split(",\\s*");   // !!! split by ","
                            if (bookInfo.length == 7) {
                                category = bookInfo[0];
                                title = bookInfo[1];
                                press = bookInfo[2];
                                publishYear = Integer.parseInt(bookInfo[3]);
                                author = bookInfo[4];
                                price = Double.parseDouble(bookInfo[5]);
                                stock = Integer.parseInt(bookInfo[6]);
                                book = new Book(category, title, press, publishYear, author, price, stock);
                                books.add(book);
                            } else
                                System.out.println("Invalid input");
                            input = scanner.nextLine();
                        }
                        res = libraryManagementSystem.storeBook(books);
                        if (res.ok)
                            System.out.println("Successfully batch store books");
                        else
                            System.out.println(res.message);
                        break;
                    case 4: // remove book
                        System.out.println("Please enter the book_id:");
                        System.out.print("Book_id:");
                        bookID = scanner.nextInt();
                        scanner.nextLine();
                        res = libraryManagementSystem.removeBook(bookID);
                        if (res.ok)
                            System.out.println("Successfully remove the book");
                        else
                            System.out.println(res.message);
                        break;
                    case 5: // modify book info
                        System.out.println("Please enter the book_id of book to be modified:");
                        System.out.print("Book_id:");
                        bookID = scanner.nextInt();
                        scanner.nextLine();
                        System.out.println("Please enter the book information to modify:");
                        System.out.println("Tip: input [Enter] to skip the modification of one item");
                        System.out.print("Category:");
                        category = scanner.nextLine();
                        System.out.print("Title:");
                        title = scanner.nextLine();
                        System.out.print("Press:");
                        press = scanner.nextLine();
                        System.out.print("PublishYear:");
                        nextline = scanner.nextLine();
                        if (nextline.equals("")) {
                            publishYear = -1;
//                            System.out.println("ohhh"); // test
                        }
                        else
                            publishYear = Integer.parseInt(nextline);
                        System.out.print("Author:");
                        author = scanner.nextLine();
                        System.out.print("Price:");
                        nextline = scanner.nextLine();
                        if (nextline.isEmpty())
                            price = -1.0;
                        else
                            price = Double.parseDouble(nextline);
                        book = new Book(category, title, press, publishYear, author, price, stock);
                        book.setBookId(bookID);
                        res = libraryManagementSystem.modifyBookInfo(book);
                        if (res.ok)
                            System.out.println("Successfully modify book");
                        else
                            System.out.println(res.message);
                        break;
                    case 6: // query book
                        BookQueryConditions conditions = new BookQueryConditions();
                        System.out.println("Please enter the conditions to query books:");
                        System.out.println("Tips: input [Enter] to skip the current condition");
                        System.out.print("Category:");
                        conditions.setCategory(scanner.nextLine());
                        System.out.print("Title:");
                        conditions.setTitle(scanner.nextLine());
                        System.out.print("Press:");
                        conditions.setPress(scanner.nextLine());
                        System.out.print("MinPublishYear:");
                        nextline = scanner.nextLine();
                        if (!nextline.isEmpty()) {
                            conditions.setMinPublishYear(Integer.parseInt(nextline));
//                            System.out.print(Integer.parseInt(nextline)); // test
                        }
                        System.out.print("MaxPublishYear:");
                        nextline = scanner.nextLine();
                        if (!nextline.isEmpty())
                            conditions.setMaxPublishYear(Integer.parseInt(nextline));
                        System.out.print("author:");
                        conditions.setAuthor(scanner.nextLine());
                        System.out.print("MinPrice:");
                        nextline = scanner.nextLine();
                        if (!nextline.isEmpty())
                            conditions.setMinPrice(Double.parseDouble(nextline));
                        System.out.print("MaxPrice:");
                        nextline = scanner.nextLine();
                        if (!nextline.isEmpty())
                            conditions.setMaxPrice(Double.parseDouble(nextline));
                        res = libraryManagementSystem.queryBook(conditions);
                        if (res.ok) {
                            BookQueryResults bookRes;
                            bookRes = (BookQueryResults) res.payload;
                            System.out.println("Successfully query books");
                            System.out.println(bookRes.getCount() + " records found:");
                            for(int i = 0; i < bookRes.getCount(); i++)
                                System.out.println("No " + (i+1) + ": " + bookRes.getResults().get(i).toString());
                        }else
                            System.out.println(res.message);
                        break;
                    case 7: // borrow book
                        System.out.println("Please enter the borrow information:");
                        System.out.print("Card_id:");
                        cardID = scanner.nextInt();
                        scanner.nextLine();
                        System.out.print("Book_id:");
                        bookID = scanner.nextInt();
                        scanner.nextLine();
                        borrow = new Borrow(bookID, cardID);
                        borrow.resetBorrowTime();   // set time, otherwise it will be set zero
                        res = libraryManagementSystem.borrowBook(borrow);
                        if (res.ok)
                            System.out.println("Successfully borrow the book");
                        else
                            System.out.println(res.message);
                        break;
                    case 8: // return book
                        System.out.println("Please enter the return information:");
                        System.out.print("Card_id:");
                        cardID = scanner.nextInt();
                        scanner.nextLine();
                        System.out.print("Book_id:");
                        bookID = scanner.nextInt();
                        scanner.nextLine();
                        borrow = new Borrow(bookID, cardID);
                        borrow.resetReturnTime();
                        res = libraryManagementSystem.returnBook(borrow);
                        if (res.ok)
                            System.out.println("Successfully return the book");
                        else
                            System.out.println(res.message);
                        break;
                    case 9: // show borrow history
                        System.out.println("Please enter the card_id to search the borrow history:");
                        System.out.print("Card_id:");
                        cardID = scanner.nextInt();
                        scanner.nextLine();
                        res = libraryManagementSystem.showBorrowHistory(cardID);
                        if (res.ok) {
                            System.out.println("Successfully search the borrow history:");
                            BorrowHistories histories = (BorrowHistories) res.payload;
                            System.out.println("Card " + cardID + ": " + histories.getCount() + " records in total:");
                            for(int i = 0; i < histories.getCount(); i++)
                                System.out.println("No " + (i+1) + ": " + histories.getItems().get(i).toString());
                        }
                        else
                            System.out.println(res.message);
                        break;
                    case 10: // register card
                        System.out.println("Please enter the information of the card to register:");
                        System.out.print("Name:");
                        name = scanner.nextLine();
                        System.out.print("Department:");
                        department = scanner.nextLine();
                        System.out.print("Type(S/T):");
                        type_str = scanner.nextLine();
                        card = new Card(cardID, name, department, Card.CardType.values(type_str));
                        res = libraryManagementSystem.registerCard(card);
                        if (res.ok)
                            System.out.println("Successfully register the card");
                        else
                            System.out.println(res.message);
                        break;
                    case 11: // remove card
                        System.out.println("Please enter the card_id to remove:");
                        System.out.print("Card_id:");
                        cardID = scanner.nextInt();
                        scanner.nextLine();
                        res = libraryManagementSystem.removeCard(cardID);
                        if (res.ok)
                            System.out.println("Successfully remove the card");
                        else
                            System.out.println(res.message);
                        break;
                    case 12: // show cards
                        res = libraryManagementSystem.showCards();
                        if (res.ok) {
                            System.out.println("Successfully show the card");
                            CardList cards = (CardList) res.payload;
                            System.out.println(cards.getCount() + " cards in total:");
                            for(int i = 0; i < cards.getCount(); i++)
                                System.out.println("No " + (i+1) + ": " + cards.getCards().get(i).toString());
                        }
                        else
                            System.out.println(res.message);
                        break;
                    case 13: // reset database
                        res = libraryManagementSystem.resetDatabase();
                        if (res.ok)
                            System.out.println("Successfully reset database");
                        else
                            System.out.println(res.message);
                        break;
                    default:
                        System.out.println("No such choice");
                        break;
                }
            }while(choice != 0);
            System.out.println("---EXIT---");

            // release database connection handler
            if (connector.release()) {
                log.info("Success to release connection.");
            } else {
                log.warning("Failed to release connection.");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
