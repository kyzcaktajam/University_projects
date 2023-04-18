package pl.edu.mimuw.matrix;

public class ConstantMatrix extends Matrix {
    private double value;
    public ConstantMatrix(Shape shape, double value) {
        this.value = value;
        this.rows = shape.rows;
        this.columns = shape.columns;
    }
    @Override
    public double get(int row, int column) {
        assert row < rows && column < columns;
        return value;
    }

    @Override
    public double[][] data() {
        double[][] res =  new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                res[i][j] = value;
            }
        }
        return res;
    }

    @Override
    public double normOne() {
        return rows * Math.abs(value);
    }

    @Override
    public double normInfinity() {
        return columns * Math.abs(value);
    }

    @Override
    public double frobeniusNorm() {
        return Math.sqrt(rows * columns * value * value);
    }
}
