package pl.edu.mimuw.matrix;

import java.util.ArrayList;

public class SparseMatrix extends Matrix{
    private ArrayList<MatrixCellValue>[] values;

    public SparseMatrix (Shape shape, MatrixCellValue ... vals) {
        super.rows = shape.rows;
        super.columns = shape.columns;
        this.values = new ArrayList [rows];
        for(int i = 0; i < rows; i++) values[i] = new ArrayList<>();

        for (int i = 0; i < vals.length; i++) {
            values[vals[i].row].add(vals[i]);
        }
    }
    @Override
    public double get(int row, int column) {
      for(MatrixCellValue v : values[row]) {
          if (v.column == column) return v.value;
      }
      return 0;
    }

    @Override
    public double[][] data() {
        double[][] res = new double[rows][columns];
        for(int i = 0; i < rows; i++) {
            for(MatrixCellValue v: values[i]) {
                res[v.row][v.column] = v.value;
            }
        }
        return res;
    }

    @Override
    public double normOne() {
        return this.toFull().normOne();
    }

    @Override
    public double normInfinity() {
        return this.toFull().normInfinity();
    }

    @Override
    public double frobeniusNorm() {
        return this.toFull().frobeniusNorm();
    }
}
