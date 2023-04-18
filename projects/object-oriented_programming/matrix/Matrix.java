package pl.edu.mimuw.matrix;

public abstract class Matrix implements IDoubleMatrix {
    protected int rows;
    protected int columns;

    @Override
    public Shape shape(){
        return Shape.matrix(rows, columns);
    }
    public IDoubleMatrix toFull() {
        return new FullMatrix(this.data());
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other) {
        return this.toFull().times(other.toFull());
    }

    @Override
    public IDoubleMatrix times(double scalar) {
        return this.toFull().times(scalar);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other) {
        return this.toFull().plus(other.toFull());
    }

    @Override
    public IDoubleMatrix plus(double scalar) {
        return this.toFull().plus(scalar);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other) {
        return this.toFull().minus(other.toFull());
    }

    @Override
    public IDoubleMatrix minus(double scalar) {
        return this.toFull().minus(scalar);
    }
    @Override
    public String toString() {
        return this.toFull().toString();
    }
}
