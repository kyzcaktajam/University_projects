import java.util.concurrent.Semaphore;
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

        private volatile int [] done;
        private Semaphore[] s = new Semaphore[COLUMNS];
        private static int [] rowSums;
        private static Thread[] threads;

        private class Helper implements Runnable {
            private int row;

            public Helper(int row) {
                this.row = row;
            }

            private void freeSem(int col) {
                for (int i = 0; i < ROWS; i++) {
                    s[col].release();
                }
            }
            @Override
            public void run() {
                for (int col = 0; col < COLUMNS; ++col) {
                    if (done[col] < ROWS) {
                        try {
                            s[col].acquire();
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                    }
                    rowSums[row] += definition.applyAsInt(row, col);
                    // System.out.println(row + " " + col);
                    done[col + 1]++;
                    if (done[col + 1] == ROWS && col < COLUMNS - 1)
                        freeSem(col + 1);
                }
            }
        }

        public int[] rowSumsConcurrent() {
            done = new int[COLUMNS + 1];
            done[0] = ROWS;
            rowSums = new int[ROWS];
            threads = new Thread[ROWS];
            for (int i = 0; i < COLUMNS; i++) {
                s[i] = new Semaphore(0);
            }
            for (int i = 0; i < ROWS; i++) {
                threads[i] = new Thread(new Helper(i));
                threads[i].start();
            }
            for (int i = 0; i < ROWS; i++) {
                try {
                    threads[i].join();
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
            return rowSums;
        }
    }


    public static void main(String[] args) {
        Matrix matrix = new Matrix(ROWS, COLUMNS, (row, column) -> {
            int a = 2 * column + 1;
            return (row + 1) * (a % 4 - 2) * a;
        });

        int[] rowSums = matrix.rowSumsConcurrent();

        for (int i = 0; i < rowSums.length; i++) {
            System.out.println(i + " -> " + rowSums[i]);
        }
    }

}
