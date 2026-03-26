# Key

## 1
- $a = 81, b = 3, c = 2, k$ doesn't matter
- $c_\mathrm{crit} = \log_3 81 / \log_3 3 = 4$
- Case 1
- $\Theta(n^4)$

## 2
- Not suitable for the master theorem. 
- Not a divide an conquer recurrence due to the $T(n - 1)$ term, we need to be dividing by a constant $B$.

## 3
- $a = 81, b = 3, c = 4, k = 1$
- $c_\mathrm{crit}=4$
- Case 2
- $\Theta(n^4 (\lg n)^2)$

## 4
- $a = 1, b = 10, c = 1$
- $c_\mathrm{crit}=0$
- Case 3
- $\Theta(n)$
- Regularity check:
  $(n / 10) \le k \cdot n$ and $k \lt 1$? Yes, let $k = 1/10$
