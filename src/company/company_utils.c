
#include "simulation.h"

#include "dbcompany.h"
#include "company_utils.h"

Vector *company_utils_get_all_active()
{

	Vector *companies = dbcompany_get_companies_vector_copy();
	Vector_ForEach(i, company, companies, Company *) {

		if (Simulation_GetLastStockPriceByCompanyId(company->company_id) < 0.01) {

			Vector_Remove(companies, i);
			i--;

		}

	}

	return companies;

}

Vector *company_utils_get_sorted()
{

	Vector *companies  = company_utils_get_all_active();
	Vector *new_vector = Vector_Create(sizeof(Company), companies->num_elements);

	Vector_Delete(companies);
	return NULL;

}

Vector *company_utils_get_random(uint32_t amount)
{

	Vector *companies = company_utils_get_all_active();
	Vector *temp = Vector_Create(sizeof(Company), amount);

	for (size_t i = 0; i < amount;i++) {



	}

	Vector_Delete(companies);

	return temp;

}

Vector *company_utils_get_lowest(uint32_t amount)
{

	Vector *companies = company_utils_get_all_active();
	Vector *temp = Vector_Create(sizeof(Company), amount);

	for (size_t i = 0; i < amount;i++) {



	}

	Vector_Delete(companies);

	return temp;

}

Vector *company_utils_get_mean(uint32_t amount)
{

	Vector *companies = company_utils_get_all_active();
	Vector *temp = Vector_Create(sizeof(Company), amount);

	for (size_t i = 0; i < amount;i++) {



	}

	Vector_Delete(companies);

	return temp;

}

Vector *company_utils_remove_duplicates(Vector *companies)
{

	Vector_ForEach(i, company_i, companies, Company *) {

		Vector_ForEach(j, company_j, companies, Company *) {

			if (i == j)
				continue;

			if (company_i->company_id == company_j->company_id) {

				Vector_Remove(companies, j);
				j--;

			}

		}

	}
	return companies;

}