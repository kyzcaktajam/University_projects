package pl.edu.mimuw.matrix;

public class AntidiagonalMatrix extends RegularSparseMatrix{

    public AntidiagonalMatrix(double[] values) {
        super.values = values;
        super.rows = values.length;
        super.columns = values.length;
    }
    @Override
    public double get(int row, int column) {
        assert row < rows && column < columns;
        if(row + column == rows - 1) return values[row];
        else return 0;
    }

    @Override
    public double[][] data() {
        double [][] res = new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                if(i + j == rows - 1) res[i][j] = values[i];
                else res[i][j] = 0;
            }
        }
        return res;
    }

    @Override
    public double normOne() {
        double res = 0;
        for(int i = 0; i < rows; i++) {
            if (Math.abs(values[i]) > res) res = Math.abs(values[i]);
        }
        return res;
    }

    @Override
    public double normInfinity() {
        return this.normOne();
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for (int i = 0; i < rows; i++) {
            res += values[i] * values[i];
        }
        return Math.sqrt(res);
    }
}
