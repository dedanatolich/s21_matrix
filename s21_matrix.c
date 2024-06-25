#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int success = 0;
  if (result == NULL) {
    success = 1;
  } else {
    if (rows < 1 || columns < 1) {
      success = 1;
      result->matrix = NULL;
    } else {
      result->rows = rows;
      result->columns = columns;
      result->matrix = (double **)calloc(rows, sizeof(double));
      if (result->matrix == NULL) {
        success = 1;
      }
    }
    if (result->matrix != NULL) {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (!result->matrix[i]) {
          for (int j = 0; j < i; j++) {
            free(result->matrix[j]);
          }
          free(result->matrix);
          success = 1;
        }
      }
    }
  }
  return success;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (s21_matrix_is_empty(A) == 0 && s21_matrix_is_empty(B) == 0 &&
      A->rows == B->rows && A->columns == B->columns) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
          res = FAILURE;
        }
      }
    }
  } else {
    res = FAILURE;
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0 && s21_matrix_is_empty(B) == 0 &&
      A->columns == B->columns && A->rows == B->rows) {
    success = s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  } else {
    success = 1;
  }
  return success;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0 && s21_matrix_is_empty(B) == 0 &&
      A->columns == B->columns && A->rows == B->rows) {
    success = s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  } else {
    success = 1;
  }
  return success;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0) {
    success = s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  } else {
    success = 1;
  }
  return success;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0 && s21_matrix_is_empty(B) == 0) {
    if (A->columns == B->rows) {
      success = s21_create_matrix(A->rows, B->columns, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          for (int k = 0; k < B->rows; k++) {
            result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
          }
        }
      }
    } else {
      success = 2;
    }
  } else {
    success = 1;
  }
  if (success != 0) {
    s21_remove_matrix(result);
  }
  return success;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0) {
    success = s21_create_matrix(A->columns, A->rows, result);
    if (success == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  } else {
    success = 1;
  }
  return success;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0 && A->rows == A->columns) {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t minor = {0};
        double det = 0.0;
        s21_create_matrix(A->columns - 1, A->rows - 1, &minor);
        s21_minor(i, j, A, &minor);
        s21_determinant(&minor, &det);
        result->matrix[i][j] = pow(-1, (i + j)) * det;
        s21_remove_matrix(&minor);
      }
    }
  } else {
    success = 1;
  }
  return success;
}

int s21_determinant(matrix_t *A, double *result) {
  int success = 0;
  if (s21_matrix_is_empty(A) == 0 && A->rows == A->columns) {
    int n = A->rows;
    double det = 1.0;
    matrix_t B = *A;
    for (int i = 0; i < n; i++) {
      double max = fabs(B.matrix[i][i]);
      int iMax = i;
      for (int j = i + 1; j < n; j++) {
        if (fabs(B.matrix[j][i]) > max) {
          max = fabs(B.matrix[j][i]);
          iMax = j;
        }
      }
      if (B.matrix[iMax][i] == 0) {
        *result = 0;
      }
      if (iMax != i) {
        for (int k = 0; k < n; k++) {
          double temp = B.matrix[i][k];
          B.matrix[i][k] = B.matrix[iMax][k];
          B.matrix[iMax][k] = temp;
        }
        det = -det;
      }
      for (int j = i + 1; j < n; j++) {
        double factor = B.matrix[j][i] / B.matrix[i][i];
        for (int k = i; k < n; k++) {
          B.matrix[j][k] -= factor * B.matrix[i][k];
        }
      }
      det *= B.matrix[i][i];
    }
    *result = det;
  } else {
    success = 1;
  }
  return success;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int success = 1;
  if (s21_matrix_is_empty(A) == 0) {
    success = 2;
    if (A->rows == 1) {
      success = s21_create_matrix(A->rows, A->rows, result);
      if (success == 0) result->matrix[0][0] = 1.0 / A->matrix[0][0];
      return success;
    }
    double det = 0.0;
    s21_determinant(A, &det);
    if (det != 0) {
      matrix_t tmp_calc = {0};
      success = s21_calc_complements(A, &tmp_calc);
      if (success == 0) {
        matrix_t tmp_trans = {0};
        success = s21_transpose(&tmp_calc, &tmp_trans);
        if (success == 0) {
          s21_mult_number(&tmp_trans, 1 / det, result);
        }
        s21_remove_matrix(&tmp_trans);
      }
      s21_remove_matrix(&tmp_calc);
    }
  }
  return success;
}

int s21_matrix_is_empty(matrix_t *matrix) {
  int success = 0;
  if (matrix == NULL || matrix->matrix == NULL || matrix->rows <= 0 ||
      matrix->columns <= 0) {
    success = 1;
  }
  return success;
}

void s21_minor(int row, int column, matrix_t *A, matrix_t *result) {
  int rowM = 0;
  int columnM = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }
    columnM = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == column) {
        continue;
      }
      result->matrix[rowM][columnM] = A->matrix[i][j];
      columnM++;
    }
    rowM++;
  }
}