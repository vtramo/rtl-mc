#include "pairwise_reduce.h"
#include <unordered_map> // should be replaced by <unordered_map>
#include <unordered_set>
#include <list>
#include <time.h>

namespace PPLUtils {

  using namespace std;
  using namespace __gnu_cxx; // for hash set and map
  using Powerset = Parma_Polyhedra_Library::Pointset_Powerset<Poly>;
  /* Efficient pairwise reduce algorithm.
   *
   * Given a set of convex polyhedra A, returns an equivalent set of convex polyhedra B
   * such that, for all convex polyhedra P1 and P2 in B, the union of P1 and P2 is not
   * a convex polyhedron.
   *
   * To do this efficiently, it computes a maintains two maps:
   *    - adjacency map: from a convex polyhedron to the set
   *                     of convex polyhedra adjacent to it
   *    - closure map: from a convex polyhedron to its topological closure
   *
   * Moreover, it maintains a queue of polyhedra to be processed.
   */

  using PPL::IO_Operators::operator<<;

  struct Poly_hash_t {
    size_t operator()( const Poly* that ) const
    {
      return reinterpret_cast<size_t>(that);
    }
    bool operator()( const Poly* that1, const Poly* that2 ) const
    {
      return that1 == that2;
    }
  };

  // A set of polyhedra pointers (values of the adjacency map, and also the queue)
  typedef unordered_set<Poly*, Poly_hash_t, Poly_hash_t> poly_set_t;

  typedef poly_set_t adjacents_t;

  // A map from a polyhedra pointer to a set of polyhedra pointers (adjacency map)
  typedef unordered_map<Poly*, adjacents_t, Poly_hash_t, Poly_hash_t> adj_map_t;
  // A map from a polyhedra pointer to a polyhedra (closure map)
  typedef unordered_map<Poly*, Poly, Poly_hash_t, Poly_hash_t> closure_map_t;
  // A list of polyhedra pointers
  typedef list<Poly*> poly_list_t;


  // Support function for debugging
  [[maybe_unused]] static void print_map(adj_map_t &adj)
  {
    cout << "Map: " << endl;

    if (adj.empty())
      cout << "empty" << endl;
    else {
      for (adj_map_t::iterator it1 = adj.begin(); it1!= adj.end(); ++it1) {
        int i=0;
        Poly *p_point = it1->first;
        cout << "(" << i << ")\t" << *p_point << ": ";

        for (adjacents_t::iterator it2 = adj[p_point].begin(); it2 != adj[p_point].end(); ++it2) {
	        cout << **it2 << "; ";
        }

        cout << endl;
      }
    }
  }

  // Support function for debugging
  [[maybe_unused]] static void print_map_summary(adj_map_t &adj)
  {
    cout << "Map : " << endl;

    if (adj.empty())
      cout << "empty" << endl;
    else {
      int i = 0;
      for (adj_map_t::iterator it1 = adj.begin(); it1!= adj.end(); ++it1) {
        Poly *p_point = it1->first;
        cout << "\t" << ++i << "(" << p_point << ") : ";

        for (adjacents_t::iterator it2 = adj[p_point].begin(); it2 != adj[p_point].end(); ++it2) {
	        cout << *it2 << "; ";
        }

        cout << endl;
      }
    }
  }


  // Initializes the adjacency map
  void init_map(Powerset &ps, adj_map_t &adj) {
    closure_map_t closure {};
    poly_list_t list {};

    adj.reserve(ps.size());
    closure.reserve(ps.size());

    // Initialize adjacency map with empty sets
    // and closure map with closures
    for (Powerset::iterator it1 = ps.begin(); it1 != ps.end(); ++it1) {
      Poly *p_point = new Poly(it1->pointset());
      list.push_back(p_point);
      adj[p_point] = adjacents_t(); // start with empty set
      Poly p_closed(*p_point);
      p_closed.topological_closure_assign();
      closure[p_point] = p_closed;
    }

    cout << "(init:" << list.size() << ")" << flush;
    int count = 0; // DEBUG

    // For each patch P
    for (poly_list_t::iterator it1 = list.begin(); it1 != list.end(); ++it1) {
      poly_list_t::iterator it2 = it1;
      Poly *p_point = *it1, &p_closed = closure[p_point];

      // For each patch Q coming after P
      while (++it2 != list.end()) {
        Poly *q_point = *it2;

        // weak boundary:
        Poly boundary(p_closed);
        boundary.intersection_assign(closure[q_point]);

        // strong boundary:
        /*
	  Poly boundary1(*q_point), boundary2(*p_point);
	  boundary1.intersection_assign(p_closed);
	  boundary2.intersection_assign(closure[q_point]);
	  if (!boundary1.is_empty() || !boundary2.is_empty()) {
        */
        if (!boundary.is_empty()) {
	  // they are adjacent!
	  count++; //DEBUG
	  adj[p_point].insert(q_point);
	  adj[q_point].insert(p_point);
        }
      }
    }

    cout << "(adjs:" << count << ")" << flush;
  }


  // Merges b into a.
  static void merge_maps(adj_map_t &a, adj_map_t &b)
  {
    closure_map_t closure;
    /* b_list is for efficiency; a_list is also for correctness,
       as "a" is modified by the second preliminary loop
     */
    poly_list_t a_list, b_list;

    // print_map_summary(a);
    // print_map_summary(b);

    // Initialize closures
    for (adj_map_t::iterator a_it = a.begin(); a_it != a.end(); ++a_it) {
      Poly p_closed(*a_it->first);
      p_closed.topological_closure_assign();
      closure[a_it->first] = p_closed;
      a_list.push_back(a_it->first);
    }
    for (adj_map_t::iterator b_it = b.begin(); b_it != b.end(); ++b_it) {
      Poly p_closed(*b_it->first);
      p_closed.topological_closure_assign();
      closure[b_it->first] = p_closed;
      b_list.push_back(b_it->first);
      a[b_it->first] = b_it->second; // copy b into a
    }

    cout << "(merging " << a_list.size() << "x" << b_list.size() << ")" << flush;

    // For each P in A
    for (poly_list_t::iterator a_it = a_list.begin(); a_it != a_list.end(); ++a_it) {
      Poly *p_point = *a_it, &p_closed = closure[p_point];

      // For each Q in B
      for (poly_list_t::iterator b_it = b_list.begin(); b_it != b_list.end(); ++b_it) {
        Poly *q_point = *b_it;
        Poly boundary1(*q_point), boundary2(*p_point);
        boundary1.intersection_assign(p_closed);
        boundary2.intersection_assign(closure[q_point]);
        if (!boundary1.is_empty() || !boundary2.is_empty()) {
	  // they are adjacent!
	  a[p_point].insert(q_point);
	  a[q_point].insert(p_point);
        }
      }
    }

    cout << "(merged)" << flush;
    // print_map_summary(a);
  }


  /* Called when P and Q are joined.
     P has already been modified to contain the union of original P and Q.
     Removes all traces of Q from the map, the queue, and from memory.
  */
  static void join_patches(Poly *p_point, Poly *q_point, adj_map_t &adj, poly_set_t &queue)
  {
    // For all adjacents R of Q
    for (adjacents_t::iterator it = adj[q_point].begin(); it != adj[q_point].end(); ++it) {
        Poly *r_point = *it;
        // cout << "Removing " << *q_point << " from adjacents of " << *r_point << endl;
        // remove Q from adjacents of R
        adj[r_point].erase(q_point);
        // add R to adjacents of P, unless R=P!
        if (r_point!=p_point) {
	  adj[p_point].insert(r_point);
	  adj[r_point].insert(p_point);
        }
    }
    adj.erase(q_point);
    queue.erase(q_point);
    delete q_point;
  }


  // adj must be already prepared
  void pairwise_reduce_map(adj_map_t &adj)
  {
    poly_set_t queue;

    // print_map(adj);

    // Initialize the queue with all patches
    queue.reserve(adj.size());
    for (adj_map_t::iterator it = adj.begin(); it!= adj.end(); it++) {
      Poly *p_point = it->first;
      queue.insert(p_point);
    }

    while (!queue.empty()) {
      // Extract first convex polyhedron P from the queue
      Poly *p_point = *queue.begin();
      queue.erase(p_point);

      // cout << "[" << queue.size() << "/" << p_point->generators().total_memory_in_bytes() << "]" << flush;

      // for each adjacent Q
      for (adjacents_t::iterator it = adj[p_point].begin(); it != adj[p_point].end(); ++it) {
        // check if their union is a convex polyhedron
        Poly *q_point = *it;

        // print_map_summary(adj);
        // cout << "checking " << *p_point << "(" << p_point << ") and " << *q_point << "(" << q_point << ") ..." << flush;

        if (p_point->upper_bound_assign_if_exact(*q_point)) {
	  // it is, so replace both of them with their union
	  // cout << "combining them!" << endl;
	  // P has already been modified, Q needs to be removed
	  join_patches(p_point, q_point, adj, queue);

	  // put modified P back into the queue
	  queue.insert(p_point);
	  break; // exits from the for cycle
        } // else? should we do something?
      }
    }

    cout << "(c:" << adj.size() << ")" << flush;
  }


  void map_to_powerset(adj_map_t &adj, Powerset &poly)
  {
    poly.clear();
    for (adj_map_t::iterator it = adj.begin(); it!= adj.end(); ++it) {
      Poly *p_point = it->first;
      poly.add_disjunct(*p_point);
    }
  }

  void cleanup_map(adj_map_t &adj)
  {
    for (adj_map_t::iterator it = adj.begin(); it!= adj.end(); ++it) {
      Poly *p_point = it->first;
      delete p_point;
    }
  }

  /* Hierarchical, no merging (no reuse of maps).
     Obsolete.
  */
  void pairwise_reduce_hyerarchical(Powerset &poly)
  {
    int max_size = 200;
    int n = poly.size(), batches = 1 + n/max_size;

    if (n<=1) return; // Nothing to reduce

    cout << "(n0: " << n  << ")" << flush;

    // Continue until there is only one batch
    while (batches>1) {
      int i = 0;
      Powerset batch(poly.space_dimension(), EMPTY), result(poly.space_dimension(), EMPTY);

      // TO DO: sort polyhedra according to some spatial criterion,
      // so it is more likely that they are adjacent within one batch

      Powerset::iterator it = poly.begin();
      while (it != poly.end()) {
        batch.add_disjunct(it->pointset());
        ++it;
        if (i++ == max_size || it==poly.end()) {
	  rawPairwiseReduce(batch);
	  // cout << "(UBA-start)" << flush;
	  for (Powerset::iterator it2 = batch.begin(); it2 != batch.end(); ++it2)
	    result.add_disjunct(it2->pointset());
	  // result.upper_bound_assign(batch);
	  // cout << "(UBA-end)" << flush;
	  batch.clear();
	  i=0;
	  cout << "," << flush;
        }
      }
      //poly.swap(result);
      // Riga successiva al commento aggiunta da Urbano poichè il metodo swap è ora chiamato m_swap.
      // m_swap scambia il poliedro dell'argomento con quello su cui è invocato.
      poly.m_swap(result);
      n = poly.size();
      max_size *= 2;
      batches = 1 + n/max_size;
      cout << "(n: " << n  << ")" << flush;
    }

    // We do this last step unconditionally, so the result is exact.
    rawPairwiseReduce(poly);
  }


  /* With bound and map.
     Map is assumed to be initially empty.
     Warning: it does not modify poly, just the map.
     Warning: the map will point to dynamically allocated memory;
              it will require memory cleanup with cleanup_map.
   */
  static void pairwise_reduce_with_bound_and_map(Powerset &poly, const int max_size, adj_map_t &map)
  {
    int n = poly.size();

    cout << "(n0: " << n  << ")" << flush;

    if (n<=max_size) {
      // base case: reduce it without splitting
      poly.pairwise_reduce();
      init_map(poly, map);
      // pairwise_reduce_map(map);
    } else {
      // split it in two halves
      Powerset  a(poly.space_dimension(), EMPTY), b(poly.space_dimension(), EMPTY);
      int i = 0, half = n/2;

      // TO DO: sort polyhedra according to some spatial criterion,
      // so it is more likely that they are adjacent within one batch
      for (Powerset::iterator it = poly.begin(); it != poly.end(); ++it)
        if (i++<half)
	        a.add_disjunct(it->pointset());
        else
	        b.add_disjunct(it->pointset());

      adj_map_t b_map;
      // recursive calls
      pairwise_reduce_with_bound_and_map(a, max_size, map);
      pairwise_reduce_with_bound_and_map(b, max_size, b_map);
      merge_maps(map, b_map);
      // TO DO: it is useless to check a polyhedron against its _old_ neighbours
      pairwise_reduce_map(map);
    }
  }

  /* Merges two reduced powersets into a single reduced powerset.
     a and b are supposed to be already reduced.
     dest is an output parameter (its content is destroyed).
     dest can be equal to a or b.
  */
  void merge_and_pairwise_reduce(Powerset &dest, Powerset &a, Powerset &b)
  {
    const dimension_type dim = dest.space_dimension();
    poly_list_t list_a, list_b;
    Powerset temp(dim, EMPTY), ab(dim, EMPTY);

    for (Powerset::iterator it=a.begin(); it!=a.end(); ++it)
      {
        // Istruzione commentata da Urbano poichè da problemi di compilazione. Cannot find operator push_back.
        //list_a.push_back(&it->pointset());

        // L'errore precedente dovrebbe essere dovuto al fatto che "push_back" vuole un valore const e non un rvalue
        // come nell'istruzione precedente che è stata commentata.
        const Poly* tmp = &it->pointset();
        list_a.push_back(const_cast<Poly*>(tmp));
      }

    for (Powerset::iterator it=b.begin(); it!=b.end(); ++it)
      {
        // Istruzione commentata da Urbano poichè da problemi di compilazione. Cannot find operator push_back.
        //list_b.push_back(&it->pointset());

        // L'errore precedente dovrebbe essere dovuto al fatto che "push_back" vuole un valore const e non un rvalue
        // come nell'istruzione precedente che è stata commentata.
        const Poly* tmp = &it->pointset();
        list_b.push_back(const_cast<Poly*>(tmp));
      }


    for (poly_list_t::iterator it_a=list_a.begin(); it_a!=list_a.end();) {
      Poly *p_point = *it_a;
      bool merge = false;
      for (poly_list_t::iterator it_b=list_b.begin(); it_b!=list_b.end(); ++it_b) {
        Poly *q_point = *it_b;
        if (p_point->upper_bound_assign_if_exact(*q_point)) {
	        temp.add_disjunct(*p_point);
	        it_a = list_a.erase(it_a);
	        list_b.erase(it_b);
	        merge = true;
	        break; // exits from b cycle
        }
      }
      // Advance it_a only if the b loop completed with no merge
      if (!merge)
        ++it_a;
    }

    for (poly_list_t::iterator it_a=list_a.begin(); it_a!=list_a.end(); ++it_a)
      ab.add_disjunct(**it_a);
    for (poly_list_t::iterator it_b=list_b.begin(); it_b!=list_b.end(); ++it_b)
      ab.add_disjunct(**it_b);

    if (!temp.empty()) {
      fastPairwiseReduce(temp);
      merge_and_pairwise_reduce(dest, ab, temp);
    } else {
      //dest.swap(ab);
      // La successiva linea di codice è stata aggiunta da Urbano poichè il metodo swap non esiste più
      // nella nuova ppl. E' stato sostituito da m_swap.
      dest.m_swap(ab);
    }
  }


  /* Merges two reduced powersets into a single reduced powerset.
     a and b are supposed to be already reduced.
     Dumb version. */
  [[maybe_unused]] static void dumb_merge_and_pairwise_reduce(Powerset &dest, const Powerset &a, Powerset &b)
  {
    dest = a;
    for (Powerset::iterator it=b.begin(); it!=b.end(); ++it)
      dest.add_disjunct(it->pointset());
    dest.pairwise_reduce();
  }


  /** This is the fastest version.
      It does not use adjacencies. */
  static void hyerarchical_ppl_pairwise_reduce(Powerset &poly)
  {
    constexpr int max_size = 20;
    int n = poly.size();

    // cout << "(hppl: " << n  << ")" << flush;

    if (n<=max_size) { // base case: reduce it without splitting
      poly.pairwise_reduce();
    } else {
      // split it in two halves
      Powerset  a(poly.space_dimension(), EMPTY), b(poly.space_dimension(), EMPTY);
      int i = 0, half = n/2;

      // TO DO: sort polyhedra according to some spatial criterion,
      // so it is more likely that they are adjacent within one batch
      for (Powerset::iterator it = poly.begin(); it != poly.end(); ++it)
        if (i++<half)
	        a.add_disjunct(it->pointset());
        else
	        b.add_disjunct(it->pointset());

      // recursive calls
      hyerarchical_ppl_pairwise_reduce(a);
      hyerarchical_ppl_pairwise_reduce(b);
      merge_and_pairwise_reduce(poly, a, b);
    }
  }


  /* Barebone version: builds a complete adjacency map beforehand.
   */
  void rawPairwiseReduce(Powerset &powerset)
  {
    adj_map_t adj;

    init_map(powerset, adj);
    pairwise_reduce_map(adj);
    map_to_powerset(adj, powerset);
    cleanup_map(adj);
  }


  void hyerarchical_pairwise_reduce_with_map(Powerset &poly, adj_map_t &adj)
  {
    constexpr int max_size = 100;
    if (poly.size()<=1) return; // Nothing to reduce
    pairwise_reduce_with_bound_and_map(poly, max_size, adj);
    map_to_powerset(adj, poly);
  }


  void hyerarchical_pairwise_reduce(Powerset &poly)
  {
    // const int max_size = 100;
    if (poly.size()<=1) return; // Nothing to reduce
    adj_map_t adj;
    hyerarchical_pairwise_reduce_with_map(poly, adj);
    cleanup_map(adj);
  }


  void metaPairwiseReduce(Powerset &powerset, const bool check_correctness /* = false */)
  {
    Powerset poly1(powerset), poly2(powerset), poly3(powerset);
    int n = powerset.size();

    clock_t start = clock();
    powerset.pairwise_reduce();
    clock_t end = clock();
    double ppl_time = (end - start) / static_cast<double>(CLOCKS_PER_SEC);

    /* start = clock();
    rawPairwiseReduce(poly1);
    end = clock();
    raw_time = (end-start)/(double)CLOCKS_PER_SEC;
    */

    /*
    if (!poly1.geometrically_covers(poly)) {
      cout << "RAW smaller than PPL!" << endl;
      exit(1);
    }
    if (!poly.geometrically_covers(poly1)) {
      cout << "PPL smaller than RAW!" << endl;
      exit(1);
    }
    */

    start = clock();
    hyerarchical_pairwise_reduce(poly2);
    end = clock();
    double fast_time = (end - start) / static_cast<double>(CLOCKS_PER_SEC);

    start = clock();
    hyerarchical_ppl_pairwise_reduce(poly3);
    end = clock();
    double hppl_time = (end - start) / static_cast<double>(CLOCKS_PER_SEC);

    cout << endl << "[" << n << ",\t"
         << ppl_time << ",\t"
      // << raw_time << ",\t"
         << fast_time << ",\t"
         << hppl_time << "]" << endl;

    if (check_correctness) {
      if (!poly3.geometrically_covers(powerset)) {
        cout << "HPPL smaller than PPL!" << endl;
        exit(1);
      }
      if (!powerset.geometrically_covers(poly3)) {
        cout << "PPL smaller than HPPL!" << endl;
        exit(1);
      }
    }
  }


  void fastPairwiseReduce(Powerset &powerset)
  {
    hyerarchical_ppl_pairwise_reduce(powerset);
  }


  /* Warning: the map keys are pointers to dynamically allocated polyhedra.
   */
  void fast_pairwise_reduce_with_map(Powerset &poly, adj_map_t &adj)
  {
    if (poly.size()<200) {
      poly.pairwise_reduce();
      init_map(poly, adj);
    }  else {
      hyerarchical_pairwise_reduce_with_map(poly, adj);
    }
  }

}