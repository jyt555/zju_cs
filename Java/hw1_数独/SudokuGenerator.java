import java.util.Random;
import java.util.Scanner;

public class SudokuGenerator {
    private static final int SIZE = 9;
    private static final int GRID = 3; // nine 3x3 grids
    private int[][] map;
    private Random rand;

    public SudokuGenerator() {
        map = new int[SIZE][SIZE];
        rand = new Random();
        fillMap();
    }

    // fillMap(): return true if the map is filled, false otherwise
    private boolean fillMap() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (map[row][col] == 0) {
                    for (int num = 1; num <= SIZE; num++) {
                        if (isSafe(row, col, num)) {
                            map[row][col] = num;
                            if (fillMap()) return true;
                            map[row][col] = 0; // backtrack
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    // isSafe(): return true if the number is safe to place in the cell, false otherwise
    private boolean isSafe(int row, int col, int num) {
        for (int x = 0; x < SIZE; x++) {
            if (map[row][x] == num || map[x][col] == num ||
                map[row / GRID * GRID + x / GRID][col / GRID * GRID + x % GRID] == num) {
                return false;
            }
        }
        return true;
    }

    // generatePuzzle(): generate a puzzle with the given number of hints
    public void generatePuzzle(int hints) {
        int count = SIZE * SIZE - hints;
        while (count != 0) {
            int i = rand.nextInt(SIZE);
            int j = rand.nextInt(SIZE);

            if (map[i][j] != 0) {
                map[i][j] = 0;
                count--;
            }
        }
    }

    // printMap(): print the map to the console
    public void printMap() {
        for (int r = 0; r < SIZE; r++) {
            System.out.print("\33[44m ");
            for (int d = 0; d < SIZE; d++) {
                if (map[r][d] == 0) System.out.print("_ ");
                else System.out.print(map[r][d] + " ");
            }
            System.out.print("\33[0m");
            System.out.print("\n");
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("\33[33mPlease enter the number of hints (1-81): \33[0m");
        int hints = scanner.nextInt();
        scanner.close();

        if (hints < 1 || hints > 81) {
            System.out.println("\33[31m[Error]the number of hints should be between 1 and 81.\33[0m");
            return;
        }

        SudokuGenerator sudoku = new SudokuGenerator();
        sudoku.generatePuzzle(hints);
        System.out.println("\33[33mThe puzzle with " + hints + " hints:\33[0m");
        sudoku.printMap();
    }
}

