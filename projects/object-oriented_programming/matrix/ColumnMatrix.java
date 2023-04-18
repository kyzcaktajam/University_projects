package pl.edu.mimuw.matrix;

public class ColumnMatrix extends RegularSparseMatrix{
    public ColumnMatrix(double[] values, int size) {
        super.values = values;
        super.rows = values.length;
        super.columns = size;
    }
    @Override
    public double get(int row, int column) {
        assert row < rows && column < columns;
        return values[row];
    }
    @Override
    public double[][] data() {
        double[][] res = new double[rows][columns];
        for(int i = 0; i< rows; i++) {
            for(int j = 0; j < columns; j++) {
                res[i][j] = values[i];
            }
        }
        return res;
    }

    @Override
    public double normInfinity() {
        double res = 0;
        for(int i = 0; i < rows; i++) {
            if(Math.abs(values[i]) > res) res = Math.abs(values[i]);
        }
        return res * columns;
    }

    @Override
    public double normOne() {
        double res = 0;
        for(int i = 0; i < rows; i++) res += Math.abs(values[i]);

        return res;
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for(int i = 0; i < rows; i++) res += values[i] * values[i];

        return Math.sqrt(res);
    }
}
