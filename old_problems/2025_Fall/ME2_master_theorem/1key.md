# Key

## 1
- $a = 2, b = 2, c = 1, k = 2$
- $c_\mathrm{crit}= \lg 2 / \lg 2 = 1$
- Case 2
- This is $\Theta(n (\lg n)^3)$

## 2
- $a = 9, b = 81, c = 1/4$
- $c_\mathrm{crit}=\log_9 9 / \log_9 81 = 1 / 2$
- Case 1
- This is $\Theta(n ^{(1/2)}) = \Theta(\sqrt n)$ 

## 3
- $a=81, b = 9, c = 3$
- $c_\mathrm{crit}=\log_9 81 / \log_9 9 = 2$
- Case 3
- Regularity criterion holds:
  $81\cdot(n/9)^3 \le kn^3$?
  $\equiv n^3/9 \le kn^3,$ let $k = 1/9 \lt 1$
- This is $\Theta(n^3)$

## 4
- $a = 1, b = 2, c$ = anything, because exponentials are $\Omega$ polynomials.
- $c_\mathrm{crit} = 0$
- Case 3
- Regularity criterion holds for any valid $k \lt 1$:
  $3^{(n / 2)} \le k3^n$

