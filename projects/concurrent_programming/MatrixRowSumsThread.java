import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.function.IntBinaryOperator;

public class MatrixRowSums {

    private static final int ROWS = 10;
    private static final int COLUMNS = 100;

    private static class Matrix {

        private final int rows;
        private final int columns;
        private final IntBinaryOperator definition;

        public Matrix(int rows, int columns, IntBinaryOperator definition) {
            this.rows = rows;
            this.columns = columns;
            this.definition = definition;
        }

        public int[] rowSums() {
            int[] rowSums = new int[rows];
            for (int row = 0; row < rows; ++row) {
                int sum = 0;
                for (int column = 0; column < columns; ++column) {
                    sum += definition.applyAsInt(row, column);
                }
                rowSums[row] = sum;
            }
            return rowSums;
        }

        private static  ConcurrentMap<Integer, Integer> rowSums = new ConcurrentHashMap<>();
        private static ConcurrentMap<Integer, Integer> colDone = new ConcurrentHashMap<>();

        private static int  result[] = new int[ROWS];

        public class Helper implements Runnable{
            private int col;
            public Helper(int col) {
                this.col = col;
            }
            @Override
            public void run() {
                for (int row = 0; row < ROWS; ++row) {
                    int val = definition.applyAsInt(row, col);
                    rowSums.computeIfPresent(row, (k, v) -> v + val);
                    colDone.computeIfPresent(row, (k, v) -> v + 1);
                    //System.out.println(row + " " + col);
                    if (colDone.getOrDefault(row, 0).equals(COLUMNS)) {
                        result[row] = rowSums.get(row);
                    }
                }
            }
        }

        public int[] rowSumsThreadsafe() {
            Thread threads[] = new Thread[COLUMNS];

            for (int i = 0; i < ROWS; ++i) {
                rowSums.putIfAbsent(i, 0);
                colDone.putIfAbsent(i, 0);
            }
            for (int i = 0; i < COLUMNS; ++i) {
                threads[i] = new Thread(new Helper(i));
                threads[i].start();
            }
            for (int i = 0; i < COLUMNS; ++i) {
                try {
                    threads[i].join();
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
            return result;
        }
    }

    public static void main(String[] args) {
        Matrix matrix = new Matrix(ROWS, COLUMNS, (row, column) -> {
            int a = 2 * column + 1;
            return (row + 1) * (a % 4 - 2) * a;
        });

        int[] rowSums = matrix.rowSumsThreadsafe();

        for (int i = 0; i < rowSums.length; i++) {
            System.out.println(i + " -> " + rowSums[i]);
        }
    }

}
