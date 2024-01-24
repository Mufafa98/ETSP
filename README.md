# ETSP
## About
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The Traveling Salesman Problem is well known for its computational complexity, motivating scientists for decades to innovateand redefine problem-solving approaches. Given a set of $n$ citiesand the distance between each pair of cities, the goal is to visit each city once and returning to the start city with a minimum distance.This problem is modeled in graph theory with the help of vertices and edges. Each city being represented through a node in the graph, and the way to other cities through its edges.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The goal of this project is to build a genetic algorithm alongside a simulated annealing algorithm form scratch and run them on TSP instances from TSPLib$^{[1]}$ that use EUC-2D$^{[2]}$ as encoding. 

## The Fitness Function
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The TSP problem stands in finding a path through all disponible cities that has a minimum distance. In order to be able to at least try to solve this problem we first need a fitness function:
$$\sum_{i = 1}^{N - 1} d(c_i, c_{i+1}) + d(c_N, c_1)$$

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The above sum represents the cost of passing through a series of cities of $N$ cities exactly once and returning to the start city. Since we need our list to contain distinct cities, as we don't want to pass through a city twice, we will use a permutation of $N$ elements representing a way that we can pass through all cities. So we need to find a set $S$ such as the above presented sum in $S$ is minimal.

## The problem with the traditional representation
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Now that we've exposed the needs for our problem, let's take a look on how a basic genetic algorithm with one point crossover would behave. Let's say we chose possition 4 to make the cut. We obtain the folowing offsprings:

| Parent 1 | Parent 2 | x | Offspring 1 | Offspring 2 |
|:--:|:--:|:--:|:--:|:--:|
| 1 | 9 | -> | 1 | 9 |
| 2 | 8 | -> | 2 | 8 |
| 3 | 7 | -> | 3 | 7 |
| 4 | 6 | -> | 4 | 6 |
| 5 | 5 | -> | 5 | 5 |
| 6 | 4 | -> | 4 | 6 |
| 7 | 3 | -> | 3 | 7 |
| 8 | 2 | -> | 2 | 8 |
| 9 | 1 | -> | 1 | 9 |

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; We can clearly see that the obtained offspring are not permutations anymore. To solve this, we have three options:

 1. The first one is leaving it in the population, but with a worse
    fitness value than of the other chromosomes. This way we are not
    Interfering with the evolutionary process, but we still have a
    chance to include in the next generations, chromosomes, that are not
    appropriate for this problem.
 2. The second option is to include the restriction in the
    representation of the input for the Fitness function, such
    as the OX encoding.
 3. A third way to solve this problem is to research specific operators 
    that may be applied to permutations without damaging them. Some
    worth    mentioning that have been used in the research of this
    project are Ordered Crossover and Swap Mutation.

## Simulated Annealing
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Simulated Annealing is one of the oldest metaheuristics in problem solving, its main objective being to generate random candidates in the search space and with a probability test being accepted or not. Its main advantage being the possibility to have a worse candidate in the next generation thus being able to escape 
local optimum points.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Below we have a pseudocode for a Simulated Annealing algorithm:
```c++
t = 0;
initialize_temperature(T);
select_at_random(current_candidate);
evaluate(current_candidate);
do
	do
		new_candidate = select_a_neighbor_of(current_candidate)
		if(Eval(vn) > Eval(vc) || random[0, 1) < exp(-abs(Eval(vn) - Eval(vc))/ T))
			vc = vn;
	while(!termination_condition);
	update_temperature();
	t++;
while(!halting_criterion);
```
  

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; In order to have an efficient Simulated Annealing algorithm we need to configure three main components: the temperature initialization process, the cooling schedule and the halting criterion.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The initialization of the temperature is dictated by how diverse we want the candidates to be, if a lower temperature is chosen, a smaller number of candidates are selected in the next generation that might be worse that the current one, the opposite is true for a higher temperature. In order to be more precise in the initialization phase we will use the following function to calculate the temperature:
$$t_0 = \Delta C \left(\ln\frac{m_2}{m_2n-(1-n)m_1}\right)^{-1}$$

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Note that $\Delta C$ represents the average fittness value for a number $N_{trials}$ of candidates. This average is then multiplied by the inverse of the natural logarithm of the raport between the number of candidates $m_2$ that lead to a worse result and the difference between the acceptance ratio $n$ multiplied with $m_2$ and the inverse of the acceptance ratio multiplied with the number of candidates $m_1$ that lead to a improvement to the current candidate.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; In order to have a efficient convergence to an optimalsolution, it would be best to have a dynamic cooling strategy. In order to achive that, we will use a set of $N_{mem}$ past candidates on which we will apply the function:
$$f(t)=t\left(1+\frac{\ln(1+\delta)t}{3\sigma(t)}\right)^{-1}$$

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Where the temperature in the next stage $f(t)$ will equal to the outcome of the current temperature $t$ multiplied by the inverse of the sum of $1$ and the raport between $t$ multiplied by the natural logarithm of $1$ and a small number $\delta$, that dictates how fast we want the temperature to decrease, and the product between $3$ and $\sigma(t)$ that represents the standard deviation of our set of past candidates, thus if the set of candidates is tends to be uniform, the temperature will decrease faster.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The Halting criterion, in our implementation is represented by the temperature reaching a certain threshold named $T_{min}$. Note that all above formulas ware inspired by $^{[3][4][5]}$ and that the initialization was omited in the code.

## Genetic Algorithm

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solving the TSP problem using a genetic algorithm is one of the most popular methods because it provides a good chance to get the best solution to an instance. Its main advantage is represented by the big population and the operators that act on it, therefore encouraging the best chromosomes to evolve and dominate the population.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Below we have a pseudocode for a Genetic Algorithm:
```c++
generation = 0;
generate_starting_population(Population, generation);
Eval(Population, generation);
while (stopping_criterion)
	generation++;
	Apply_Operators(Population, generation - 1);
	Eval(Population, generation - 1);
	ApplySelection(Population, generation)
```
  

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The evaluation of the population is calculated by adding the distance between each two adjacent cities and between the start and the end city for each Chromosome.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; In order to get a better starting point, the initial population was initialized by a Nearest-Neighbor strategy, thus, selecting a random city, and from that we select the next city based on the shortest distance between them, this process is repeated for each city that is last in our chromosome until there is no other city to go to. This operation is needed because of the huge search space, therefore, helping the genetic algorithm to get closer to a optimal solution, rather than wasting time in a space far from the optim. Other worth mentioning strategies in initializing the population are: Greedy, Clarke-Wright and Christofides $^{[6]}$.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Since in our implementation, both including the restrictions in representation and using specific operators such as OX and Swap mutation have failed to give satisfactory results, we have chosen to make some research in the Inver-Over operator proposed by Guo Tao and Zbigniew Michalewicz in their paper$^{[7]}$. The main idea of this operator is to automatically balance the exploration and exploitation.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Below is presented a simplified version of it:
```c++
for(individual S in P)
S_prim = S;
c = Select_random_city_from(S_prim);
do
	if(rand(0, 1) <= p)
		c_prim = Select_city_different_than(c, S_prim);
	else
		S_temp = Select_random_individual(P);
		c_prim = Next_city_to(c, S_prim);
	if(Next_city_to(c, S_prim) == c_prim || Previous_city_to(x, S_prim) == c_prim)
		Inverse(c, c_prim, S_prim);//inverse the selection from c to c_prim in S_prim
	c = c_prim;
while(true);
if(Eval(S_prim) <= Eval(S))
	S = S_prim;
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The inversion is made from the position of city c excluding to the position of city c\_prim including. Therefore, we can represent the chromosome without any restrictions in the representation, because when we apply the 
operator, a chromosome is modified considering only its data, therefore we could not break the properties of a permutation if the chromosome initially was a permutation.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  One noticeable problem in this operator is that it eliminates all the variation in the population, therefore lowering the overall performance of the genetic algorithm. One way to fix this was proposed by some researchers in $^{[8][9]}$. The main idea in those is to include a probability by which the city is changed in the end and on the selected individual should be applied a mutation before doing any other operation with it. So below we've modified the above algorithm to include those improvements:
```c++
for(individual S in P)
S_prim = S;
do
	if(rand(0, 1) <= p_prim)
		ApplyMutation(S_prim);
		c = Select_random_city_from(S_prim);
	if(rand(0, 1) <= p)
		c_prim = Select_city_different_than(c, S_prim);
	else
		S_temp = Select_random_individual(P);
		c_prim = Next_city_to(c, S_prim);
	if(Next_city_to(c, S_prim) == c_prim || Previous_city_to(x, S_prim) == c_prim)
		Inverse(c, c_prim, S_prim);//inverse the selection from c to c_prim in S_prim
	if(rand(0, 1) <= p_sec)
		c = c_prim;
while(true);
if(Eval(S_prim) <= Eval(S))
S = S_prim
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; For ease of use we will notate p as $p_{io}$, p\_prim $p_{mut}$ and p\_sec as $p_{cc}$. The $p_{io}$ and $p_{mut}$ are given as parameters, but it would be better if we would change the city more frequently at the start and then less and less while we approach the end of the algorithm, this is being achieved by using the following formula:
$$p_{cc} = e^{\log(\frac{p_{cc-max} - p_{cc-min}}{N})} \cdot generation$$

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  Where $p_{cc-max}$ and $p_{cc-min}$ represent the maximum and the minimum value of the $p_{cc}$ parameters and $N$ represents the maximum number of generations that the algorithm will run for.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  The mutation operator that was used in this algorithm is 2-Opt that was described in $^{[10]}$. The main idea of this mutation operator is to cut two bridge between four cities and rewire them in other order that they were initially, therefore introducing more diversity in the population.

## References

 1. [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/)
 2. [EUC-2D(pg.2)](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf)
 3. Aarts, E.H.L., Korst, J.H.M. and van Laarhoven, P.J.M. A
    quantitative analysis of the simulated annealing algorithm: A case
    study for the traveling salesman problem. J Stat Phys 50, 187–206
    (1988), doi: 10.1007/bf01022991.
 4. Aarts, E., Korst, J., and Michiels, W. (n.d.). Simulated Annealing.
    Search Methodologies, 187–210. doi:10.1007/0-387-28356-0\_7
 5. Aarts, E. H. L., and Laarhoven, P. J. M. (1989). Simulated
    annealing: An introduction. Statistica Neerlandica, 43(1), 31–52.
    doi:10.1111/j.1467-9574.1989.tb01245
 6. Johnson, D. S., and McGeoch, L. A. (1997). The traveling salesman
    problem: A case study in local optimization. Local search in
    combinatorial optimization, 1(1), 215-310.
 7. Tao, G., and Michalewicz, Z. (1998). Inver-over operator for the
    TSP. Parallel Problem Solving from Nature — PPSN V, 803–812.
    doi:10.1007/ bfb0056922
 8. Wang, Y., Li, J., Gao, K., and Pan, Q. (2011). Memetic Algorithm
    based on Improved Inver–over operator and Lin–Kernighan local search
    for the Euclidean traveling salesman problem. Computers and
    Mathematics with Applications, 62(7), 2743–2754.
    doi:10.1016/j.camwa.2011.06.063
 9. Singh, D. R., Singh, M. K., and Singh, T. (2016). A Hybrid Algorithm
    with Modified Inver-Over Operator and Genetic Algorithm Search for
    Traveling Salesman Problem. Advances in Intelligent Systems and
    Computing, 141–150.doi:10.1007/978-981-10-1023-1\_14
 10. Sugumaran, V., Xu, Z., and Zhou, H. (Eds.). (2021). Application of
     Intelligent Systems in Multi-modal Information Analytics. Advances
     in Intelligent Systems and Computing.
     doi:10.1007/978-3-030-74814-2, 404-409

