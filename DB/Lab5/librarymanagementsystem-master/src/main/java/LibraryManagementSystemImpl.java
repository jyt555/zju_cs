import entities.Book;
import entities.Borrow;
import entities.Card;
import queries.*;
import utils.DBInitializer;
import utils.DatabaseConnector;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class LibraryManagementSystemImpl implements LibraryManagementSystem {

    private final DatabaseConnector connector;

    public LibraryManagementSystemImpl(DatabaseConnector connector) {
        this.connector = connector;
    }

    @Override
    public ApiResult storeBook(Book book) {
        Connection conn = connector.getConn();
        try {
            // this book has existed ?
            String sql1 = "SELECT * FROM book WHERE category = ? AND title = ? AND press = ? AND publish_year = ? AND author = ?";
            PreparedStatement stmt1 = conn.prepareStatement(sql1);
            stmt1.setString(1, book.getCategory());
            stmt1.setString(2, book.getTitle());
            stmt1.setString(3, book.getPress());
            stmt1.setInt(4, book.getPublishYear());
            stmt1.setString(5, book.getAuthor());
            ResultSet res = stmt1.executeQuery();
            if (res.next())
                return new ApiResult(false, "Book already exists");
            else {
                // new book
                String sql2 = "INSERT INTO book (category,title,press,publish_year,author,price,stock) VALUES (?, ?, ?, ?, ?, ?, ?)";
                PreparedStatement stmt2 = conn.prepareStatement(sql2);
                stmt2.setString(1, book.getCategory());
                stmt2.setString(2, book.getTitle());
                stmt2.setString(3, book.getPress());
                stmt2.setInt(4, book.getPublishYear());
                stmt2.setString(5, book.getAuthor());
                stmt2.setDouble(6, book.getPrice());
                stmt2.setInt(7, book.getStock());
                stmt2.executeUpdate();
            }
            res = stmt1.executeQuery(); // re_query the inserted book
            if (res.next())
                book.setBookId(res.getInt("book_id"));
            else {
                return new ApiResult(false, "Failed to get book id");
            }
            commit(conn);
            return new ApiResult(true, book.getBookId());
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult incBookStock(int bookId, int deltaStock) {
        Connection conn = connector.getConn();
        try {
            String sql1 = "SELECT * FROM book WHERE book_id = ?";
            PreparedStatement stmt1 = conn.prepareStatement(sql1);
            stmt1.setInt(1, bookId);
            ResultSet res = stmt1.executeQuery();
            if (!res.next()) // not exist
                return new ApiResult(false, "Can not find the book");
            int stock = res.getInt("stock");
            if (stock + deltaStock < 0)
                return new ApiResult(false, "Stock can't be negative number");
            String sql2 = "UPDATE book SET stock = ? WHERE book_id = ?";
            PreparedStatement stmt2 = conn.prepareStatement(sql2);
            stmt2.setInt(1, stock + deltaStock);
            stmt2.setInt(2, bookId);
            stmt2.executeUpdate();
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult storeBook(List<Book> books) {
        Connection conn = connector.getConn();
        try {
            String query_sql = "SELECT * FROM book WHERE category = ? AND title = ? AND press = ? AND publish_year = ? AND author = ?";
            String insert_sql = "INSERT INTO book (category,title,press,publish_year,author,price,stock) VALUES (?, ?, ?, ?, ?, ?, ?)";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            PreparedStatement insert_stmt = conn.prepareStatement(insert_sql);
            for (Book book : books) {
                query_stmt.setString(1, book.getCategory());
                query_stmt.setString(2, book.getTitle());
                query_stmt.setString(3, book.getPress());
                query_stmt.setInt(4, book.getPublishYear());
                query_stmt.setString(5, book.getAuthor());
                ResultSet res = query_stmt.executeQuery();
                if (res.next()) {
                    rollback(conn); // rollback all the books
                    return new ApiResult(false, "Some of the books already exist");
                }
                insert_stmt.setString(1, book.getCategory());
                insert_stmt.setString(2, book.getTitle());
                insert_stmt.setString(3, book.getPress());
                insert_stmt.setInt(4, book.getPublishYear());
                insert_stmt.setString(5, book.getAuthor());
                insert_stmt.setDouble(6, book.getPrice());
                insert_stmt.setInt(7, book.getStock());
                insert_stmt.addBatch(); // <
            }
            insert_stmt.executeBatch(); // >
            for (Book book : books) { // set book id
                query_stmt.setString(1, book.getCategory());
                query_stmt.setString(2, book.getTitle());
                query_stmt.setString(3, book.getPress());
                query_stmt.setInt(4, book.getPublishYear());
                query_stmt.setString(5, book.getAuthor());
                ResultSet new_res = query_stmt.executeQuery();
                if (new_res.next())
                    book.setBookId(new_res.getInt("book_id"));
                else {
                    rollback(conn);
                    return new ApiResult(false, "Failed to insert the  book");
                }
            }
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult removeBook(int bookId) {
        Connection conn = connector.getConn();
        try {
            String query_sql = "SELECT * FROM book WHERE book_id = ?";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, bookId);
            ResultSet res = query_stmt.executeQuery();
            if (!res.next())
                return new ApiResult(false, "Can not find the book");
            query_sql = "SELECT * FROM borrow WHERE book_id = ? AND return_time = 0";
            query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, bookId);
            res = query_stmt.executeQuery();
            if (res.next())
                return new ApiResult(false, "Book is borrowed");
            String remove_sql = "DELETE FROM book WHERE book_id = ?";
            PreparedStatement remove_stmt = conn.prepareStatement(remove_sql);
            remove_stmt.setInt(1, bookId);
            remove_stmt.executeUpdate();
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult modifyBookInfo(Book book) {
        Connection conn = connector.getConn();
        try {
            String query_sql = "SELECT * FROM book WHERE book_id = ?";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, book.getBookId());
            ResultSet res = query_stmt.executeQuery();
            if (!res.next())
                return new ApiResult(false, "Can not find the book");
            String modify_sql = "UPDATE book SET category = ?, title = ?, press = ?, publish_year = ?, author = ?, price = ? WHERE book_id = ?";
            PreparedStatement modify_stmt = conn.prepareStatement(modify_sql);
            if (!book.getCategory().isEmpty())
                modify_stmt.setString(1, book.getCategory());
            else
                modify_stmt.setString(1, res.getString("category"));
            if (!book.getTitle().isEmpty())
                modify_stmt.setString(2, book.getTitle());
            else
                modify_stmt.setString(2, res.getString("title"));
            if (!book.getPress().isEmpty())
                modify_stmt.setString(3, book.getPress());
            else
                modify_stmt.setString(3, res.getString("press"));
            if (book.getPublishYear() != -1)
                modify_stmt.setInt(4, book.getPublishYear());
            else
                modify_stmt.setInt(4, res.getInt("publish_year"));
            if (!book.getAuthor().isEmpty())
                modify_stmt.setString(5, book.getAuthor());
            else
                modify_stmt.setString(5, res.getString("author"));
            if (book.getPrice() != -1.0)
                modify_stmt.setDouble(6, book.getPrice());
            else
                modify_stmt.setDouble(6, res.getDouble("price"));
            modify_stmt.setInt(7, book.getBookId());
            modify_stmt.executeUpdate();
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult queryBook(BookQueryConditions conditions) {
        Connection conn = connector.getConn();
        List<Book> books = new ArrayList<>();
        try {
            String sql = "SELECT * FROM book WHERE title like ? AND press like ? AND publish_year >= ? AND publish_year <= ? AND author like ? AND price >= ? AND price <= ? AND category like ?";
            PreparedStatement stmt = conn.prepareStatement(sql);
            if (conditions.getTitle() != null)
                stmt.setString(1, "%"+conditions.getTitle()+"%");
            else
                stmt.setString(1, "%"); // no parameter
            if (conditions.getPress() != null)
                stmt.setString(2, "%"+conditions.getPress()+"%");
            else
                stmt.setString(2, "%");
            if (conditions.getMinPublishYear() != null)
                stmt.setInt(3, conditions.getMinPublishYear());
            else
                stmt.setInt(3, -1);
            if (conditions.getMaxPublishYear() != null)
                stmt.setInt(4, conditions.getMaxPublishYear());
            else
                stmt.setInt(4, 10000);
            if (conditions.getAuthor() != null)
                stmt.setString(5, "%"+conditions.getAuthor()+"%");
            else
                stmt.setString(5, "%");
            if (conditions.getMinPrice() != null)
                stmt.setDouble(6, conditions.getMinPrice());
            else
                stmt.setDouble(6, -1);
            if (conditions.getMaxPrice() != null)
                stmt.setDouble(7, conditions.getMaxPrice());
            else
                stmt.setDouble(7, 10000);
            if (conditions.getCategory() != null)
                stmt.setString(8, "%"+conditions.getCategory()+"%");
            else
                stmt.setString(8, "%");
            ResultSet res = stmt.executeQuery();
            while (res.next()) {
                Book book = new Book();
                book.setCategory(res.getString("category"));
                book.setTitle(res.getString("title"));
                book.setPress(res.getString("press"));
                book.setPublishYear(res.getInt("publish_year"));
                book.setAuthor(res.getString("author"));
                book.setPrice(res.getDouble("price"));
                book.setStock(res.getInt("stock"));
                book.setBookId(res.getInt("book_id"));
                books.add(book);
            }
            if (conditions.getSortOrder() == SortOrder.ASC) // asc !!!
                books.sort(conditions.getSortBy().getComparator());
            else
                books.sort(conditions.getSortBy().getComparator().reversed());  // desc
            commit(conn);           
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
        BookQueryResults result = new BookQueryResults(books);
        return new ApiResult(true, result);
    }

    @Override
    public ApiResult borrowBook(Borrow borrow) {
        Connection conn = connector.getConn();
        try { // !!!
            conn.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
        } catch (Exception e) {
            return new ApiResult(false, "Unimplemented Function");
        }
        try {
            String query_sql = "SELECT * FROM book WHERE book_id = ?";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, borrow.getBookId());
            ResultSet borrow_book = query_stmt.executeQuery();
            if (!borrow_book.next())
                return new ApiResult(false, "Can not find the book");
            if (borrow_book.getInt("stock") <= 0) {
                rollback(conn);
                return new ApiResult(false, "No stock");
            } else {
                String query_borrow_sql = "SELECT * FROM borrow WHERE book_id = ? AND card_id = ? AND return_time = 0";
                query_stmt = conn.prepareStatement(query_borrow_sql);
                query_stmt.setInt(1, borrow.getBookId());
                query_stmt.setInt(2, borrow.getCardId());
                ResultSet record = query_stmt.executeQuery();
                if(record.next()) {
                    rollback(conn);
                    return new ApiResult(false, "You had borrowed this book");
                } else {
                    String borrow_sql = "INSERT INTO borrow (card_id, book_id, borrow_time) VALUES(?,?,?)";
                    PreparedStatement borrow_stmt = conn.prepareStatement(borrow_sql);
                    borrow_stmt.setInt(1, borrow.getCardId());
                    borrow_stmt.setInt(2, borrow.getBookId());
                    borrow_stmt.setLong(3, borrow.getBorrowTime());
                    borrow_stmt.executeUpdate();
                    String delta_stock_sql = "UPDATE book SET stock = ? WHERE book_id = ?";
                    PreparedStatement delta_stock_stmt = conn.prepareStatement(delta_stock_sql);
                    delta_stock_stmt.setInt(1, borrow_book.getInt("stock")-1);
                    delta_stock_stmt.setString(2, borrow_book.getString("book_id"));
                    delta_stock_stmt.executeUpdate();
                }
            }
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult returnBook(Borrow borrow) {
        Connection conn = connector.getConn();
        try {
            String query_sql = "SELECT * FROM borrow WHERE book_id = ? AND card_id = ? AND return_time = 0";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, borrow.getBookId());
            query_stmt.setInt(2, borrow.getCardId());
            ResultSet res = query_stmt.executeQuery();
            if (!res.next())
                return new ApiResult(false, "No borrow record");
            else
                borrow.setBorrowTime(res.getLong("borrow_time")); // do not be given the borrow time
            if (borrow.getReturnTime() <= borrow.getBorrowTime())
                return new ApiResult(false, "Invalid return time");
            String update_sql = "UPDATE borrow SET return_time = ? WHERE book_id = ? AND card_id = ? AND return_time = 0";
            String inc_sql = "UPDATE book SET stock = stock+1 WHERE book_id = ?";
            PreparedStatement up_stmt = conn.prepareStatement(update_sql);
            PreparedStatement inc_stmt = conn.prepareStatement(inc_sql);
            up_stmt.setLong(1, borrow.getReturnTime());
            up_stmt.setInt(2, borrow.getBookId());
            up_stmt.setInt(3, borrow.getCardId());
            int result = up_stmt.executeUpdate();
            if (result == 0) {
                rollback(conn);
                return new ApiResult(false, "No borrow record");
            }
            inc_stmt.setInt(1, borrow.getBookId());
            inc_stmt.executeUpdate();
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult showBorrowHistory(int cardId) {
        Connection conn = connector.getConn();
        List<BorrowHistories.Item> histories = new ArrayList<>();
        try {
            String query_sql = "SELECT * FROM borrow WHERE card_id = ? ORDER BY borrow_time DESC, book_id ASC";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, cardId);
            ResultSet records = query_stmt.executeQuery();
            while (records.next()) {    // scan every book
                String book_sql = "SELECT * FROM book WHERE book_id = ?";
                PreparedStatement book_stmt = conn.prepareStatement(book_sql);
                book_stmt.setInt(1, records.getInt("book_id"));
                ResultSet book = book_stmt.executeQuery();
                if (book.next()) {
                    Book new_book = new Book();
                    new_book.setCategory(book.getString("category"));
                    new_book.setTitle(book.getString("title"));
                    new_book.setPress(book.getString("press"));
                    new_book.setPublishYear(book.getInt("publish_year"));
                    new_book.setAuthor(book.getString("author"));
                    new_book.setPrice(book.getDouble("price"));
                    new_book.setStock(book.getInt("stock"));
                    new_book.setBookId(book.getInt("book_id"));
                    Borrow new_borrow = new Borrow(cardId, new_book.getBookId());
                    new_borrow.setBorrowTime(records.getLong("borrow_time"));
                    new_borrow.setReturnTime(records.getLong("return_time"));
                    BorrowHistories.Item item = new BorrowHistories.Item(cardId, new_book, new_borrow);
                    histories.add(item);
                }
            }
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
        BorrowHistories result = new BorrowHistories(histories);
        return new ApiResult(true, result);
    }

    @Override
    public ApiResult registerCard(Card card) {
        Connection conn = connector.getConn();
        try {
            String query_sql = "SELECT * FROM card WHERE name = ? AND department = ? AND type = ?";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setString(1, card.getName());
            query_stmt.setString(2, card.getDepartment());
            query_stmt.setString(3, card.getType().getStr());
            ResultSet res = query_stmt.executeQuery();
            if (res.next()) {
                rollback(conn);
                return new ApiResult(false, "Card already exists");
            } else {
                String insert_sql = "INSERT INTO card (name, department, type) VALUES (?, ? ,?)";
                PreparedStatement insert_stmt = conn.prepareStatement(insert_sql);
                insert_stmt.setString(1, card.getName());
                insert_stmt.setString(2, card.getDepartment());
                insert_stmt.setString(3, card.getType().getStr());
                insert_stmt.executeUpdate();
                res = query_stmt.executeQuery();
                if (res.next())
                    card.setCardId(res.getInt("card_id"));
            }
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult removeCard(int cardId) {
        Connection conn = connector.getConn();
        try {
            String query_sql = "SELECT * FROM borrow WHERE card_id = ? AND return_time = 0";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            query_stmt.setInt(1, cardId);
            ResultSet res = query_stmt.executeQuery();
            if (res.next())
                return new ApiResult(false, "There are un-returned books");
            String remove_sql = "DELETE FROM card WHERE card_id = ?";
            PreparedStatement remove_stmt = conn.prepareStatement(remove_sql);
            remove_stmt.setInt(1, cardId);
            int del = remove_stmt.executeUpdate();
            if (del == 0) {
                rollback(conn);
                return new ApiResult(false, "The card to be deleted doesn't exist");
            }
            commit(conn);
            return new ApiResult(true, null);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
    }

    @Override
    public ApiResult showCards() {
        Connection conn = connector.getConn();
        List<Card> cards = new ArrayList<>();
        try {
            String query_sql = "SELECT * FROM card ORDER BY card_id";
            PreparedStatement query_stmt = conn.prepareStatement(query_sql);
            ResultSet res = query_stmt.executeQuery();
            while (res.next()) {
                Card card = new Card();
                card.setCardId(res.getInt("card_id"));
                card.setName(res.getString("name"));
                card.setDepartment(res.getString("department"));
                card.setType(Card.CardType.values(res.getString("type")));  // card type !!!
                cards.add(card);
            }
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, "Unimplemented Function");
        }
        CardList result = new CardList(cards);
        return new ApiResult(true, result);
    }

    
    @Override
    public ApiResult resetDatabase() {
        Connection conn = connector.getConn();
        try {
            Statement stmt = conn.createStatement();
            DBInitializer initializer = connector.getConf().getType().getDbInitializer();
            stmt.addBatch(initializer.sqlDropBorrow());
            stmt.addBatch(initializer.sqlDropBook());
            stmt.addBatch(initializer.sqlDropCard());
            stmt.addBatch(initializer.sqlCreateCard());
            stmt.addBatch(initializer.sqlCreateBook());
            stmt.addBatch(initializer.sqlCreateBorrow());
            stmt.executeBatch();
            commit(conn);
        } catch (Exception e) {
            rollback(conn);
            return new ApiResult(false, e.getMessage());
        }
        return new ApiResult(true, null);
    }

    private void rollback(Connection conn) {
        try {
            conn.rollback();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void commit(Connection conn) {
        try {
            conn.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
