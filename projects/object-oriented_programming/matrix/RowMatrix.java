package pl.edu.mimuw.matrix;

public class RowMatrix extends RegularSparseMatrix{

    public RowMatrix(double[] values, int size) {
        super.values = values;
        super.rows = size;
        super.columns = values.length;
    }
    @Override
    public double get(int row, int column) {
        assert row < rows && column < columns;
        return values[column];
    }
    @Override
    public double[][] data() {
        double[][] res = new double[rows][columns];
        for(int i = 0; i< rows; i++) {
            for(int j = 0; j < columns; j++) {
                res[i][j] = values[j];
            }
        }
        return res;
    }

    @Override
    public double normOne() {
        double res = 0;
        for(int i = 0; i < columns; i++) {
            if(Math.abs(values[i]) > res) res = Math.abs(values[i]);
        }
        return res * rows;
    }

    @Override
    public double normInfinity() {
        double res = 0;
        for(int i = 0; i < columns; i++) res += Math.abs(values[i]);

        return res;
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for(int i = 0; i < columns; i++) res += values[i] * values[i];

        return Math.sqrt(res);
    }
}
