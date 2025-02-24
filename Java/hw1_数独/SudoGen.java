import java.util.Random;
import java.util.Scanner;

public class SudoGen {
    private static final int SIZE = 9;
    private int[][] board;
    private int[][] unsolved;
    private boolean[][] row, col, box; // check if num is used in row, col, box
    private int[][] mask;
    private int hint;

    // constructor
    public SudoGen(int[][] mask, int hint) {
        this.mask = mask;
        this.hint = hint;
        board = new int[SIZE][SIZE];
        unsolved = new int[SIZE][SIZE];
        row = new boolean[SIZE][SIZE + 1];
        col = new boolean[SIZE][SIZE + 1];
        box = new boolean[SIZE][SIZE + 1];
    }

    public void generate() {
        fillGrid(0, 0);
    }

    private boolean fillGrid(int r, int c) {
        if (r == SIZE) return true;
        if (c == SIZE) return fillGrid(r + 1, 0); // next row
        if (board[r][c] != 0) return fillGrid(r, c + 1); // skip used cell

        int boxIndex = mask[r][c] - 1;

        for (int num = 1; num <= 9; num++) {
            if (!row[r][num] && !col[c][num] && !box[boxIndex][num]) {
                // fill cell
                board[r][c] = num; // [todo] fill at random
                row[r][num] = col[c][num] = box[boxIndex][num] = true;
                // iterate next cell
                if (fillGrid(r, c + 1)) return true;
                // backtrack
                board[r][c] = 0;
                row[r][num] = col[c][num] = box[boxIndex][num] = false;
            }
        }
        return false;
    }

    public void generateUnsolved() {
        for (int r = 0; r < SIZE; r++) // copy board to unsolved
            for (int c = 0; c < SIZE; c++)
                unsolved[r][c] = board[r][c];

        int count = SIZE * SIZE - hint;
        while (count != 0) { // clear the grid in unsolved board with random index
            int i = new Random().nextInt(SIZE);
            int j = new Random().nextInt(SIZE);
            if (board[i][j] != 0) {
                unsolved[i][j] = 0;
                count--;
            }
        }
    }

    public void printBoard(int[][] map) {
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                // System.out.print(board[r][c] + " ");
                int boxIndex = mask[r][c] - 1;
                switch(boxIndex) {
                    case 0: System.out.print("\33[41m"); break;
                    case 1: System.out.print("\33[42m"); break;
                    case 2: System.out.print("\33[43m"); break;
                    case 3: System.out.print("\33[44m"); break;
                    case 4: System.out.print("\33[45m"); break;
                    case 5: System.out.print("\33[46m"); break;
                    case 6: System.out.print("\33[47m"); break;
                    case 7: System.out.print("\33[48m"); break;
                    case 8: System.out.print("\33[42m"); break;
                }
                printGrid(r, c, map);
                System.out.print("\33[0m");
            }
            System.out.println();
        }
    }

    public void printGrid(int r, int c, int[][] map) {
        if (map[r][c] == 0) System.out.print("_ ");
        else System.out.print(map[r][c] + " ");
    }

    // main method
    public static void main(String[] args) {
        int[][] mask = {
            {1, 2, 2, 3, 3, 3, 4, 4, 4},
            {1, 2, 2, 3, 3, 3, 4, 4, 4},
            {1, 2, 2, 2, 3, 3, 4, 4, 4},
            {1, 2, 5, 2, 5, 3, 6, 6, 6},
            {1, 1, 5, 5, 5, 5, 5, 6, 6},
            {1, 1, 1, 8, 5, 9, 5, 9, 6},
            {7, 7, 7, 8, 8, 9, 9, 9, 6},
            {7, 7, 7, 8, 8, 8, 9, 9, 6},
            {7, 7, 7, 8, 8, 8, 9, 9, 6}
        };
        int hint = 23; // default value

        Scanner scanner = new Scanner(System.in);
        System.out.print("\33[33mPlease enter the number of hints (1-81, default 23): \33[0m");
        hint = scanner.nextInt();
        if (hint < 1 || hint > 81) {
            System.out.println("\33[31m[Error]the number of hints should be between 1 and 81.\33[0m");
            System.out.println("\33[33mUsing default value 23.\33[0m");
            hint = 23;
        }
        System.out.println("\33[33mPlease enter the mask (9x9, enter 0 to use default mask):\33[0m");
        mask:
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                int num = scanner.nextInt();
                if (r == 0 && c == 0 && num == 0)
                    break mask;
                mask[r][c] = num;
            }
        }
        scanner.close();

        System.out.println("\33[33mGenerating board...\33[0m");
        SudoGen sudo = new SudoGen(mask, hint);
        sudo.generate();
        sudo.generateUnsolved();
        sudo.printBoard(sudo.unsolved);
        System.out.println("\33[33m=====Solved board=====\33[0m");
        sudo.printBoard(sudo.board);
    }
}
