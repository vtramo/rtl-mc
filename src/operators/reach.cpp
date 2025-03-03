#include "reach.h"
#include "ppl_utils.h"
#include "pairwise_reduce.h"
#include <unordered_map>

using PPL::IO_Operators::operator<<;
using namespace PPLUtils;

PowersetUniquePtr reach0(const Poly& A, const Powerset& B, const Poly& preFlow)
{
    return reach0(Powerset { A }, B, preFlow);
}

PowersetUniquePtr reach0(const Powerset& A, const Powerset& B, const Poly& preFlow)
{
    assert(A.space_dimension() == B.space_dimension());
    assert(A.space_dimension() == preFlow.space_dimension());

    PowersetUniquePtr result { std::make_unique<Powerset>(A.space_dimension(), PPL::EMPTY) };

    for (const auto& bWrapper: B)
    {
        const Poly& b { bWrapper.pointset() };
        for (const auto& [Q, X]: reach0(A, b, preFlow))
        {
            result->add_disjunct(X);
        }
    }

    return result;
}

PowersetUniquePtr reachPlus(const Powerset& A, const Powerset& B, const Poly& preFlow)
{
    assert(A.space_dimension() == B.space_dimension());
    assert(A.space_dimension() == preFlow.space_dimension());

    assert(A.space_dimension() == B.space_dimension());
    assert(A.space_dimension() == preFlow.space_dimension());

    PowersetUniquePtr result { std::make_unique<Powerset>(A.space_dimension(), PPL::EMPTY) };

    for (const auto& bWrapper: B)
    {
        const Poly& b { bWrapper.pointset() };
        for (const auto& [Q, X]: reachPlus(A, b, preFlow))
        {
            result->add_disjunct(X);
        }
    }

    return result;
}


ReachPairs reach0(const Powerset& A, const Poly& B, const Poly& preFlow)
{
    assert(A.space_dimension() == B.space_dimension());
    assert(A.space_dimension() == preFlow.space_dimension());

    Poly preB { B };
    Poly closureB { B };
    preB.positive_time_elapse_assign(preFlow);
    closureB.topological_closure_assign();
    preB.intersection_assign(closureB);

    assert(preB.space_dimension() == closureB.space_dimension());
    assert(A.space_dimension() == preFlow.space_dimension());

    ReachPairs result {};
    result.reserve(A.size());
    for (Powerset::const_iterator patchesAit { A.begin() }; patchesAit != A.end(); ++patchesAit) {
        const Poly& Q { patchesAit->pointset() };
        Poly Y { Q };
        Y.intersection_assign(preB);
        assert(Y.space_dimension() == preFlow.space_dimension());
        assert(Q.space_dimension() == preFlow.space_dimension());
        if (!Y.is_empty()) {
            std::pair<Poly, Poly> pair {};
            pair.first = Q;
            pair.second.m_swap(Y);
            assert(pair.second.space_dimension() == preFlow.space_dimension());
            result.push_back(std::move(pair));
        }
    }

    return result;
}

ReachPairs reachPlus(const Powerset& A, const Poly& B, const Poly& preFlow)
{
    Powerset delta { preFlow.space_dimension(), PPL::EMPTY };
    Powerset notA { preFlow.space_dimension(), PPL::UNIVERSE };
    notA.difference_assign(A);
    assert(delta.space_dimension() == preFlow.space_dimension());
    assert(notA.space_dimension() == preFlow.space_dimension());

    /* Two differences between rwa(B, NotA) and reach+(A,B):
       1) reach+ requires positive delay
       2) ??
     */
    for (Powerset::const_iterator patchesAit { A.begin() }; patchesAit != A.end(); ++patchesAit) {
        Poly U { patchesAit->pointset() };
        U.topological_closure_assign();
        U.intersection_assign(B);
        U.positive_time_elapse_assign(preFlow);
        U.intersection_assign(patchesAit->pointset());
        assert(U.space_dimension() == preFlow.space_dimension());

        Powerset UU { std::move(U) };
        assert(UU.space_dimension() == preFlow.space_dimension());
        auto X { rwaMaps(UU, notA, preFlow) };
        Powerset firstPowerset { X.first };
        assert(firstPowerset.space_dimension() == preFlow.space_dimension());
        fusion(delta, firstPowerset);
        assert(delta.space_dimension() == preFlow.space_dimension());
    }

    // Find the A-patch of each part of Delta
    ReachPairs result {};
    for (Powerset::const_iterator Delta_it = delta.begin(); Delta_it != delta.end(); ++Delta_it) {
        bool patchFound {};
        const Poly& Y { Delta_it->pointset() };
        assert(Y.space_dimension() == preFlow.space_dimension());
        for (Powerset::const_iterator patchesAit { A.begin() }; patchesAit != A.end(); ++patchesAit) {
            const Poly& Q { patchesAit->pointset() };
            assert(Q.space_dimension() == preFlow.space_dimension());
            Poly Y2 { Y };
            assert(Y2.space_dimension() == preFlow.space_dimension());
            Y2.intersection_assign(Q);
            assert(Y2.space_dimension() == preFlow.space_dimension());
            if (!Y2.is_empty()) {
                patchFound = true;
                std::pair<Poly, Poly> pair {};
                pair.first = Q;
                assert(pair.first.space_dimension() == preFlow.space_dimension());
                pair.second.m_swap(Y2);
                assert(pair.second.space_dimension() == preFlow.space_dimension());
                result.push_back(std::move(pair));
            }
        }

        if (!patchFound) {
            std::cout << "A: " << A << '\n';
            std::cout << "B: " << B << '\n';
            std::cout << "Delta: " << Delta_it->pointset() << '\n';
            throw std::runtime_error { "ERROR: Patch not found!" };
        }
    }

    for (const auto & [p1, p2] : result)
        assert(p1.space_dimension() == preFlow.space_dimension());

    return result;
}


typedef std::list<PolySharedPtr> list_nnc_type;
typedef std::unordered_map<PolySharedPtr, list_nnc_type> hash_map_type;

static void init_maps(
    Powerset& risky_ps,
    Powerset& notover_ps,
    Poly flow_nnc,
    hash_map_type& intadj,
    hash_map_type& extadj,
    list_nnc_type& queue
);

static std::pair<Powerset, Powerset> refine_maps(
    Powerset &U_ps,
    Poly flow_nnc,
    hash_map_type& intadj,
    hash_map_type& extadj,
    list_nnc_type& queue
);

/* Returns a partition of "not V" into a pair of polyhedra (A, B):
   - A is the set of points that may reach U while avoiding V
   - B contains the other points of not V
   So, A \/ B = not V

   This version admits a finite number of sharp corners in each trajectory.
   It uses adjacency maps.
*/
std::pair<Powerset, Powerset> rwaMaps(Powerset U, Powerset V, Poly preFlow)
{
    PPL::dimension_type dim { U.space_dimension() };
    hash_map_type intadj {};
    hash_map_type extadj {};
    list_nnc_type queue {};
    Powerset notV { dim, PPL::UNIVERSE };
    V.add_space_dimensions_and_embed(dim - V.space_dimension());

    // CALCOLA NOT_V
    notV.difference_assign(V);
    notV.difference_assign(U);
    fastPairwiseReduce(notV);

    init_maps(notV, U, preFlow, intadj, extadj, queue);
    return refine_maps(U, preFlow, intadj, extadj, queue);
}

Poly simple_border_opt(const Poly &poly1_nnc, const Poly &poly1close_nnc, const Poly &poly2_nnc);
Poly exit_border_opt(const Poly &internal_poly_nnc, Poly &internal_polyclose_nnc, const Poly &external_poly_nnc, const Poly &flow_nnc);

double num = 0;
static void init_maps(
    Powerset& risky_ps,
    Powerset& notover_ps,
    Poly flow_nnc,
    hash_map_type& intadj,
    hash_map_type& extadj,
    list_nnc_type& queue
)
{
    PPL::dimension_type dim=risky_ps.space_dimension();
    hash_map_type::iterator it1_intadj, it2_intadj;
    Powerset::iterator it_risky, it_notover;
    PolySharedPtr p1_point {};
    PolySharedPtr p2_point {};
    PolySharedPtr b_point {};
    Poly p1close_nnc(dim), pprime_nnc(dim), b_nnc(dim), elapse_nnc(dim);
    list_nnc_type list_nnc {};
    bool first_ext;

    // INSERISCI TUTTI GLI ELEMENTI DI NOT_V NELLA HASH MAP INTADJ
    for (it_risky=risky_ps.begin(); it_risky!=risky_ps.end(); ++it_risky)
    {
        p1_point = std::make_shared<Poly>(it_risky->pointset());
        intadj[p1_point]=list_nnc;
    }

    // PER OGNI P1 IN INTADJ
    for (it1_intadj=intadj.begin(); it1_intadj!=intadj.end(); ++it1_intadj)
    {
        p1_point = it1_intadj->first;
        p1close_nnc=*p1_point;
        p1close_nnc.topological_closure_assign();

        //PER OGNI COPPIA P2<>P1 in INTADJ
        it2_intadj=it1_intadj;
        ++it2_intadj;
        while (it2_intadj!=intadj.end())
        {
            p2_point = it2_intadj->first;
            b_nnc = simple_border_opt(*p1_point, p1close_nnc, *p2_point);
            num++;

            // SE IL BNDRY NON E' VUOTO, ALLORA P1 E P2 SONO ADIACENTI
            if (!b_nnc.is_empty())
            {
                intadj[p1_point].push_back(p2_point);
                intadj[p2_point].push_back(p1_point);
            }
            ++it2_intadj;
        } // ENDFOR P2

        // PER OGNI P' IN W
        first_ext=true;
        for (it_notover=notover_ps.begin(); it_notover!=notover_ps.end(); it_notover++)
        {
            pprime_nnc = it_notover->pointset();
            b_nnc = exit_border_opt(*p1_point, p1close_nnc, pprime_nnc, flow_nnc);
            num++;

            // SE ENTRY NON E' VUOTO, INSERISCI P1 ED ENTRY in extadj
            if (!b_nnc.is_empty())
            {
                if (first_ext)
                {
                    extadj[p1_point]=list_nnc;
                    queue.push_back(p1_point);
                    first_ext=false;
                }
                b_point = std::make_shared<Poly>(b_nnc);
                extadj[p1_point].push_back(b_point);
            }
        } // ENDFOR P'
    } // ENDFOR P1
}

Poly simple_border_opt(const Poly &poly1_nnc, const Poly &poly1close_nnc, const Poly &poly2_nnc)
{
    //Calcola bndry(p1,p2)
    Poly border_nnc(poly1close_nnc);

    border_nnc.intersection_assign(poly2_nnc);
    if (border_nnc.is_empty())
    {
        Poly closure2_nnc(poly2_nnc);
        closure2_nnc.topological_closure_assign();
        closure2_nnc.intersection_assign(poly1_nnc);
        return closure2_nnc;
    }
    return border_nnc;
}

Poly bndry(const Poly &ph1,const Poly &ph2);

Poly exit_border_opt(
    const Poly &internal_poly_nnc,
    [[maybe_unused]] Poly &internal_polyclose_nnc,
    const Poly &external_poly_nnc,
    const Poly &flow_nnc
)
{
    //calcola entry(p1,p2)
    Poly border_nnc=bndry(internal_poly_nnc, external_poly_nnc);
    if (!border_nnc.is_empty())
    {
        Poly elapse_nnc(external_poly_nnc);
        elapse_nnc.time_elapse_assign(flow_nnc);
        border_nnc.intersection_assign(elapse_nnc);
    }
    return border_nnc;
}

Poly bndry(const Poly &ph1,const Poly &ph2)
{
    //conta_bndry++; COMMENTATO DA MARCO.
    //copia ph1
    Poly output(ph1);
    //copia ph2
    Poly cl_ph2(ph2);

    //cl(ph1) (chiusura topologica di ph1)
    output.topological_closure_assign();

    //cl(ph2)
    cl_ph2.topological_closure_assign();

    //cl(ph1) /\ ph2 (cl(ph1) intersecato ph2)
    output.intersection_assign(ph2);

    //cl(ph2) /\ ph1
    cl_ph2.intersection_assign(ph1);

    //(cl(ph1) /\ ph2)\/(cl(ph2) /\ ph1) ((cl(ph1) /\ ph2) unito (cl(ph2) /\ ph1))
    output.upper_bound_assign_if_exact(cl_ph2);

    return output;
}

static Powerset cut_polyhedra_with_exit_borders(PolySharedPtr p_point, const Poly &flow_nnc, hash_map_type& extadj);
static void update_int_first(list_nnc_type &mylist_nnc, PolySharedPtr p2_point, hash_map_type &intadj);
static void update_external(Powerset &cut_ps, PolySharedPtr p2_point, Poly &flow_nnc, hash_map_type &extadj, list_nnc_type &queue);
static void update_int_succ(list_nnc_type &mylist_nnc, PolySharedPtr p2_point, hash_map_type &intadj);
static void update_int_ext(Powerset &pnew_ps, Powerset &cut_ps, PolySharedPtr p1_point, Poly &flow_nnc, hash_map_type &intadj, hash_map_type &extadj, list_nnc_type &queue);
static void update_int(Powerset &pnew_ps, hash_map_type &intadj);

/*
LOOP PRINCIPALE
*/
static std::pair<Powerset, Powerset> refine_maps(
    Powerset &U_ps,
    Poly flow_nnc,
    hash_map_type& intadj,
    hash_map_type& extadj,
    list_nnc_type& queue
)
{
    PPL::dimension_type dim=flow_nnc.space_dimension();
    PolySharedPtr p_point {};
    Poly p_nnc(dim), b_nnc(dim), pprime_nnc(dim), pprime_close_nnc(dim);
    Powerset cut_ps(dim), totalcut_ps(dim, PPL::EMPTY);
    [[maybe_unused]] Powerset::iterator it_under;
    Powerset W(U_ps);
    hash_map_type::iterator it_int;
    list_nnc_type::iterator it_list;

    // ANALIZZA QUEUE
    while (!queue.empty())
    {
        // ESTRAI IL POLIEDRO P DA QUEUE
        p_point=queue.front();
        // ELIMINA P DA QUEUE
        queue.pop_front();
        p_nnc = *p_point;

        // CALCOLA CUT
        cut_ps = cut_polyhedra_with_exit_borders(p_point, flow_nnc, extadj);
        // SE CUT NON E' VUOTO AGGIORNA LE MAP
        if (!cut_ps.empty())
        {
            W.upper_bound_assign(cut_ps);
            Powerset pnew_ps(dim, PPL::EMPTY);
            pnew_ps.add_disjunct(p_nnc);
            // CALCOLA P \ CUT CHE SONO I PUNTI DI P CHE NON SONO AGGIUNTI AL RISULTATO
            pnew_ps.difference_assign(cut_ps);
            fastPairwiseReduce(pnew_ps);

            // CONTROLLA SE P HA ADIACENTI INTERNI
            if (!intadj[p_point].empty())
                // AGGIORNA ADIACENZE INTERNE E ESTERNE ( SE P HA ADIACENTI)
                    update_int_ext(pnew_ps, cut_ps, p_point, flow_nnc, intadj, extadj, queue);
            else
                // AGGIORNA ADIACENZE  INTERNE (PERCHE P NON HA ADIACENTI)
                    update_int(pnew_ps, intadj);

            // RIMUOVI P DA INTADJ
            intadj.erase(p_point);
        } // END IF
    } // END WHILE

    fastPairwiseReduce(W);

    for (it_int=intadj.begin(); it_int!=intadj.end(); ++it_int)
        totalcut_ps.add_disjunct(*(it_int->first));
    fastPairwiseReduce(totalcut_ps);

    return std::make_pair(W, totalcut_ps);
}

// P /\ PREFLOW(B) CON B={b | (P,b) app extadj}
static Powerset cut_polyhedra_with_exit_borders(
    PolySharedPtr p_point,
    const Poly &flow_nnc,
    hash_map_type& extadj
)
{
    // Variabile globale conta_reach commentata, per comodità di compilazione.
    //conta_reach++;
    PPL::dimension_type dim=flow_nnc.space_dimension();
    Powerset b_ps(dim, PPL::EMPTY), flow_ps(flow_nnc), p_ps(*p_point);

    // Cut polyhedron with exit borders
    // Collect the exit borders
    for (list_nnc_type::iterator it_list=extadj[p_point].begin(); it_list!=extadj[p_point].end(); ++it_list)
        b_ps.add_disjunct(**it_list);


    // Compute pre-flow of border set
    b_ps.time_elapse_assign(flow_ps);
    fastPairwiseReduce(b_ps);
    p_ps.intersection_assign(b_ps);
    fastPairwiseReduce(p_ps);
    return p_ps;
}

//AGGIORNAMENTO DELLE ADIACENZE INTERNE E ESTERNE
static void update_int_ext(
    Powerset &pnew_ps,
    Powerset &cut_ps,
    PolySharedPtr p1_point,
    Poly &flow_nnc,
    hash_map_type &intadj,
    hash_map_type &extadj,
    list_nnc_type &queue
)
{
    bool first=true;
    list_nnc_type mylist_nnc, list_nnc;

    //INSERISCE TUTTI I POLIEDRI DI PNEW IN INTADJ E IN MYLIST
    for (Powerset::iterator it_pnew=pnew_ps.begin(); it_pnew!=pnew_ps.end(); ++it_pnew)
    {
        PolySharedPtr q_point { std::make_shared<Poly>(it_pnew->pointset()) };
        intadj[q_point]=list_nnc;
        mylist_nnc.push_back(q_point);
    }
    // PER OGNI P2 ADIACENTE A P1
    for (list_nnc_type::iterator it_list=intadj[p1_point].begin(); it_list!=intadj[p1_point].end(); ++it_list)
    {
        PolySharedPtr p2_point = *it_list;

        //RIMUOVI P1 DALLA LISTA DELLE ADIACENZE DI P2(PERCHÈ ADESSO P1 STARA IN W??)
        intadj[p2_point].remove(p1_point);
        // First step of update AGGIORNA LE ADIACENZE DI P2 CONTROLLANDO ANCHE QUELLE TRA POLIEDRI DI PNEW
        if (first)
        {
            update_int_first(mylist_nnc, p2_point, intadj);
            first=false;
        }
        else // Second step AGGIORNA LE ADIACENZE DI P2 SENZA CONTROLLARE PIU QUELLE TRA POLIEDRI DI PNEW
            update_int_succ(mylist_nnc, p2_point, intadj);

        update_external(cut_ps, p2_point, flow_nnc, extadj, queue);
    }
}

// PRIMO PASSO DELL'AGGIORNAMENTO DELLE ADIACENZE INTERNE DI P2 (ADIACENTE A P): CONTROLLA LE ADIACENZE TRA(P2, Pnew) E (Pnew, Pnew)
static void update_int_first(list_nnc_type &mylist_nnc, PolySharedPtr p2_point, hash_map_type &intadj)
{
    list_nnc_type::iterator it1_list, it2_list;
    list_nnc_type list_nnc;
    PolySharedPtr q1_point {};
    PolySharedPtr q2_point {};
    [[maybe_unused]] Powerset:: iterator it_pnew;

    // PER OGNI Q1 IN MYLIST (PER OGNI Q1 IN PNEW)
    for (it1_list=mylist_nnc.begin(); it1_list!=mylist_nnc.end(); ++it1_list)
    {
        q1_point = *it1_list;
        Poly q1close_nnc(*q1_point);
        q1close_nnc.topological_closure_assign();

        Poly b_nnc (simple_border_opt(*q1_point, q1close_nnc, *p2_point));
        num++;

        //SE Q1 E P2 SONO ADIACENTI, INSERISCI Q1 E Q2 IN INTADJ
        if (!b_nnc.is_empty())
        {
            intadj[q1_point].push_back(p2_point);
            intadj[p2_point].push_back(q1_point);
        }

        //PER OGNI Q2 DIVERSO DA Q1 IN PNEW
        it2_list=it1_list;
        ++it2_list;
        while (it2_list!=mylist_nnc.end())
        {
            q2_point = *it2_list;
            b_nnc = simple_border_opt(*q1_point, q1close_nnc, *q2_point);
            num++;

            //SE Q1 E Q2 SONO ADIACENTI INSERISCI Q1 E Q2 IN INTADJ
            if (!b_nnc.is_empty())
            {
                intadj[q1_point].push_back(q2_point);
                intadj[q2_point].push_back(q1_point);
            }
            ++it2_list;
        } // ENDFOR Q2
    } // ENDFOR Q1
}

//SECONDO PASSO DELL'AGGIORNAMENTO DELLE ADIACENZE INTERNE DI P2 ( ADIACENTE A P): CONTROLLA LE ADIACENZE SOLO TRA (P2,PNEW) SENZA CONTOLLARE ANCHE QUELLE TRA (PNEW,PNEW)
static void update_int_succ(list_nnc_type &mylist_nnc, PolySharedPtr p2_point, hash_map_type &intadj)
{
    // PER OGNI Q1 IN MYLIST (PER OGNI Q1 IN PNEW)
	for (list_nnc_type::iterator it_list = mylist_nnc.begin(); it_list!=mylist_nnc.end(); ++it_list)
    {
		PolySharedPtr q_point = *it_list;
		Poly qclose_nnc(*q_point);
		qclose_nnc.topological_closure_assign();

		Poly b_nnc(simple_border_opt(*q_point, qclose_nnc, *p2_point));
		num++;
		// SE P2 E Q1 SONO ADIACENTI INSERISCILI IN INTADJ
		if (!b_nnc.is_empty())
		{
			intadj[q_point].push_back(p2_point);
			intadj[p2_point].push_back(q_point);
		}
	} // ENDFOR Pnew
}

//AGGIORNA LE ADIACENZE ESTERNE DI P2 (ADIACENTE ALL'ORIGINALE P) CONTROLLANDO I BNDRY TRA P2 E IL CUT CALCOLATO DA P
static void update_external(Powerset &cut_ps, PolySharedPtr p2_point, Poly &flow_nnc, hash_map_type &extadj, list_nnc_type &queue)
{
	PPL::dimension_type dim = flow_nnc.space_dimension();
    PolySharedPtr b_point {};
    Poly q1_nnc(dim), p2close_nnc(*p2_point);
	Powerset:: iterator it_cut;

	p2close_nnc.topological_closure_assign();

	//PER OGNI Q1 IN CUT
	for (it_cut=cut_ps.begin(); it_cut!=cut_ps.end(); ++it_cut)
    {
		q1_nnc = it_cut->pointset();
		Poly b_nnc(exit_border_opt(*p2_point, p2close_nnc, q1_nnc, flow_nnc));
		num++;
		//SE LA ENTRY REGION CON P2 NON È VUOTA AGGIORNA L'ADIACENZE ESTERNE E METTE P2 NELLA CODA
		if (!b_nnc.is_empty())
		{
            list_nnc_type list_nnc;
            b_point = std::make_shared<Poly>(b_nnc);
			if (extadj.find(p2_point)==extadj.end())
				extadj[p2_point]=list_nnc;

			extadj[p2_point].push_back(b_point);

			bool find=false;

			// Scan queue
			list_nnc_type::iterator it_queue=queue.begin();
			while (it_queue!=queue.end() && (!find))
			{
				if (*it_queue==p2_point)
					find=true;
				++it_queue;
			}
			// INSERISCI P2 NELLA QUEQUE SOLO SE NON E' GIA PRESENTE
			if (!find)
				queue.push_back(p2_point);
		}
	} // ENDFOR cut
}

//AGGIORNA LE ADIACENZE INTERNE SOLO TRA POLIEDRI DI PNEW . VIENE CHIAMATO QUANDO IL P ORIGINALE NON HA ADIACENTI
// Updates internal adjacencies among pieces of "Pnew".
// Called when the original polyhedron being cut has no adjacents.
static void update_int(Powerset &pnew_ps, hash_map_type &intadj)
{
    list_nnc_type mylist_nnc;
    list_nnc_type::iterator it1_list, it2_list;
    PolySharedPtr q1_point {};
    PolySharedPtr q2_point {};
    Powerset::iterator it_pnew;

    //INSERISCE TUTTI I POLIEDRI DI PNEW IN INTADJ
    //PER OGNI Q1 IN PNEW
    for (it_pnew=pnew_ps.begin(); it_pnew!=pnew_ps.end(); ++it_pnew)
    {
        list_nnc_type list_nnc;
        q1_point = std::make_shared<Poly>(it_pnew->pointset());
        // INIZIALIZZA LA LISTA DI ADIACENZA DI Q1
        intadj[q1_point]=list_nnc;
        mylist_nnc.push_back(q1_point);
    }

    // PER OGNI Q1 IN PNEW
    for (it1_list=mylist_nnc.begin(); it1_list!=mylist_nnc.end(); ++it1_list)
    {
        q1_point = *it1_list;
        Poly q1close_nnc(*q1_point);
        q1close_nnc.topological_closure_assign();

        // PER OGNI COPPIA Q1<>Q2 IN PNEW
        it2_list=it1_list;
        ++it2_list;
        while (it2_list!=mylist_nnc.end())
        {
            q2_point = *it2_list;
            //CALCOLA BNDRY(Q1,Q2)
            Poly b_nnc(simple_border_opt(*q1_point, q1close_nnc, *q2_point));
            num++;

            // SE Q1 E Q2 SONO ADIACENTI, INSERISCI Q1 E Q2 IN INTADJ
            if (!b_nnc.is_empty())
            {
                intadj[q1_point].push_back(q2_point);
                intadj[q2_point].push_back(q1_point);
            }
            ++it2_list;
        } // ENDFOR Q2
    } // ENDFOR Q1
}
