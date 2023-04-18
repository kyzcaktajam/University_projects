package pl.edu.mimuw.matrix;

public class DoubleMatrixFactory {

  private DoubleMatrixFactory() {
  }

  public static IDoubleMatrix sparse(Shape shape, MatrixCellValue... values){
    return new SparseMatrix(shape, values);
  }

  public static IDoubleMatrix full(double[][] values) {
    return new FullMatrix(values);
  }

  public static IDoubleMatrix identity(int size) {
    double [] values = new double[size];
    for(int i = 0; i < size; i++) values[i] = 1;
    return new DiagonalMatrix(values);
  }

  public static IDoubleMatrix diagonal(double... diagonalValues) {
    return new DiagonalMatrix(diagonalValues);
  }

  public static IDoubleMatrix antiDiagonal(double... antiDiagonalValues) {
    return new AntidiagonalMatrix(antiDiagonalValues);
  }

  public static IDoubleMatrix vector(double... values) {
    double [][] vals = new double[values.length][1];
    for (int i = 0; i < values.length; i++) vals[i][0] = values[i];
    return new FullMatrix(vals);
  }

  public static IDoubleMatrix zero(Shape shape) {
    return new ConstantMatrix(shape, 0);
  }

  public static IDoubleMatrix constant(Shape shape, double value) {
    return new ConstantMatrix(shape, value);
  }


  public static IDoubleMatrix rowMatrix(double[] values, int second_coordinate) {
    return new RowMatrix(values, second_coordinate);
  }

  public static IDoubleMatrix columnMatrix(double[] values, int second_coordinate) {
    return new ColumnMatrix(values, second_coordinate);
  }
}
