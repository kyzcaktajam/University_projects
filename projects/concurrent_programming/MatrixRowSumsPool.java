import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.function.IntBinaryOperator;

public class MatrixRowSums {
    private static final int ROWS = 10;
    private static final int COLUMNS = 100;
    private static final int T = 4;

    private static class Matrix {
        private final int rows;
        private final int columns;
        private static IntBinaryOperator definition;

        public Matrix(int rows, int columns, IntBinaryOperator definition) {
            this.rows = rows;
            this.columns = columns;
            this.definition = definition;
        }
        private static class Element {
            int row;
            int col;
            public Element(int row, int col) {
                this.row = row;
                this.col = col;
            }
            public int getRow() {
                return this.row;
            }
            public int getCol() {
                return this.col;
            }
            public int getVal() {
                return definition.applyAsInt(row, col);
            }
        }
        private static class Calc implements Callable<Element> {

            private final int row;
            private final int col;

            public Calc(int row, int col) {
                this.row = row;
                this.col = col;
            }

            @Override
            public Element call() throws Exception {
                return new Element(row, col);
            }
        }

        private static void rowSums() {
            ExecutorService pool = Executors.newFixedThreadPool(T);
            List<Callable<Element>>  calcs = new ArrayList<>();

            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLUMNS; c++) {
                    calcs.add(new Calc(r, c));
                }
            }

            try {
                List<Future<Element> > res = pool.invokeAll(calcs);
                int sum = 0;

                for (Future<Element> i : res) {
                    Element e = i.get();
                    sum += e.getVal();
                    if (e.getCol() == COLUMNS - 1) {
                        System.out.println(e.getRow() + ": " + sum);
                        sum = 0;
                    }

                }

            } catch (ExecutionException e) {
                throw new RuntimeException(e);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            } finally {
                pool.shutdown();
            }
        }
    }

    public static void main(String[] args) {
        Matrix matrix = new Matrix(ROWS, COLUMNS, (row, column) -> {
            int a = 2 * column + 1;
            return (row + 1) * (a % 4 - 2) * a;
        });

        matrix.rowSums();
    }

}
