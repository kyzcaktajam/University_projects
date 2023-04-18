package pl.edu.mimuw.matrix;

public class DiagonalMatrix extends RegularSparseMatrix{

    public DiagonalMatrix(double[] values) {
        super.values = values;
        super.rows = values.length;
        super.columns = values.length;
    }

    //optymalizacja względem zwracanego typu:
    @Override
    public IDoubleMatrix plus (IDoubleMatrix other) {
        if(other.getClass() == this.getClass()) {
            assert rows == ((DiagonalMatrix) other).rows;
            double [] val = new double[rows];
            for(int i = 0; i < rows; i++) {
                val[i] = values[i] + other.get(i, i);
            }
            return new DiagonalMatrix(val);
        }
        else return super.plus(other);
    }

    @Override
    public double get(int row, int column) {
        assert row < rows && column < columns;
        if(row == column) return values[row];
        else return 0;
    }

    @Override
    public double[][] data() {
        double[][] val = new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                if(i == j) val[i][j] = values[i];
                else val[i][j] = 0;
            }
        }
        return val;
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

    @Override
    public String toString() {
        String res = rows + " " + columns + '\n';
        res = res + values[0] + "...\n";
        if(rows > 1) res = res + 0 + ' ' +  values[1] + "...\n";
        if(rows > 2) res = res + 0 + ' ' + 0 + ' ' +  values[2] + "...\n";
        for (int i = 3; i < rows - 3; i++) {
            res = res + "... " + values[i] + " ...\n";
        }
        if(rows > 5) res = res + "... " +  values[rows - 3] + " 0 0\n";
        if(rows > 4) res = res + "... " +  values[rows - 3] + " 0\n";
        if(rows > 3) res = res + "... " +  values[rows - 3] + "\n";
        return res;
    }
}
