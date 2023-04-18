package pl.edu.mimuw.matrix;

public class FullMatrix extends Matrix{
    private double [][] values;

    public FullMatrix(double[][] val){
        this.values = val;
        super.rows = val.length;
        super.columns = val[0].length;
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other) {
        IDoubleMatrix full = other.toFull();
        assert columns == full.shape().rows;
        double[][] result = new double[rows][full.shape().columns];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < full.shape().columns; j++) {
                double currVal = 0;
                for (int k = 0; k < columns; k++) {
                    currVal += values[i][k] * full.get(k, j);
                }
                result[i][j] = currVal;
            }
        }
        return new FullMatrix(result);
    }

    @Override
    public IDoubleMatrix times(double scalar) {
        double[][] result = new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                result[i][j] = values[i][j] * scalar;
            }
        }
        return new FullMatrix(result);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other) {
        FullMatrix full = new FullMatrix(other.data());
        assert rows == full.shape().rows && columns == full.shape().columns;
        double[][] result = new double[rows][full.shape().columns];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; ++j) {
                result[i][j] = values[i][j] + full.get(i, j);
            }
        }
        return new FullMatrix(result);
    }

    @Override
    public IDoubleMatrix plus(double scalar) {
        double[][] result = new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                result[i][j] = values[i][j] + scalar;
            }
        }
        return new FullMatrix(result);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other) {
        return this.plus(new FullMatrix(other.data()).times(-1));
    }

    @Override
    public IDoubleMatrix minus(double scalar) {
        double[][] result = new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                result[i][j] = values[i][j] - scalar;
            }
        }
        return new FullMatrix(result);
    }

    @Override
    public double get(int row, int column) {
        assert row < rows && column < columns;
        return values[row][column];
    }

    @Override
    public double[][] data() {
        return values;
    }

    @Override
    public double normOne() {
        double result = 0;
        for(int j = 0; j < columns; j++) {
            double sum = 0;
            for(int i = 0; i < rows; i ++) {
                sum += Math.abs(values[i][j]);
            }
            if (sum > result) result = sum;
        }
        return result;
    }

    @Override
    public double normInfinity() {
        double result = 0;
        for(int i = 0; i < rows; i++) {
            double sum = 0;
            for(int j = 0; j < columns; j++) {
                sum += Math.abs(values[i][j]);
            }
            if (sum > result) result = sum;
        }
        return result;
    }

    @Override
    public double frobeniusNorm() {
        double result = 0;
        for(int i = 0; i< rows; i++) {
            for (int j = 0; j < columns; j++) {
                result += values[i][j] * values[i][j];
            }
        }
        return Math.sqrt(result);
    }

    @Override
    public String toString() {
        String res = rows + " " + columns + '\n';
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                res = res +  values[i][j] + ' ';
            }
            res = res + '\n';
        }
        return res;
    }

}
