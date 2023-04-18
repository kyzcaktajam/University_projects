package pl.edu.mimuw;

import pl.edu.mimuw.matrix.*;

public class Main {
  private static void show(IDoubleMatrix matrix){
    System.out.println(matrix.toString() + '\n');
  }

  public static void main(String[] args) {
    double [][] tab2d = new double[10][10];
    double [] tab1d = new double[10];
    for(int i = 0; i< 10; i++) {
      tab1d[i] = i + 1;
      for(int j = 0; j < 10; j++) {
        tab2d[i][j] = 10 * i + j;
      }
    }


    IDoubleMatrix m1, m2;
    m1 = new DiagonalMatrix(tab1d);
    show(m1);
    m1 = m1.plus(m1);
    show(m1);

    m1 = new ColumnMatrix(tab1d, 10);
    show(m1);
    System.out.println("Norma Frobeniusa: " + m1.frobeniusNorm() + '\n');

    m1 = new RowMatrix(tab1d, 10);
    show(m1);
    m1 = m1.minus(10);
    show(m1);
    m1 = new ConstantMatrix(Shape.matrix(10, 10), 17);
    show(m1);
    m1 = m1.times(m1);
    show(m1);
    m1 = new FullMatrix(tab2d);
    show(m1);
    m1 = m1.minus(m1);
    show(m1);
    m1 = new AntidiagonalMatrix(tab1d);
    show(m1);
    m1 = m1.plus(m1.plus(m1));
    show(m1);
    }
}

