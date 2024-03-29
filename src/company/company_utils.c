
#include "simulation.h"

#include "log.h"

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

bool company_utils_is_active(uint32_t company_id)
{

	Vector *companies = company_utils_get_all_active();
	bool active = false;

	Vector_ForEach(i, company, companies, Company *) {

		if (company->company_id == company_id) {

			active = true;
			break;
			
		}

	}

	Vector_Delete(companies);
	return active;

}

void company_utils_sort_company_ids(uint32_t *company_ids, Vector *companies)
{

	const size_t num_elements = companies->num_elements;

	float prices[num_elements];
	Vector_ForEach(i, company, companies, Company *) {

		company_ids[i] = company->company_id;
		prices[i] = Simulation_GetLastStockPriceByCompanyId(company->company_id);

	}

	for (size_t i = 0;i < num_elements-1;i++) {

		uint32_t temp_idx = i;
		for (size_t j = i + 1;j < num_elements;j++) {

			if (prices[j] < prices[temp_idx])
				temp_idx = j;

		}
		if (temp_idx != i) {

			uint32_t temp_company_id = company_ids[i];
			float price_temp         = prices[i];

			company_ids[i] = company_ids[temp_idx];
			company_ids[temp_idx] = temp_company_id;

			prices[i] = prices[temp_idx];
			prices[temp_idx] = price_temp;

		}

	}

}

Vector *company_utils_get_sorted()
{

	Vector *companies  = company_utils_get_all_active();
	Vector *new_vector = Vector_Create(sizeof(Company), companies->num_elements);

	uint32_t company_ids[companies->num_elements];
	company_utils_sort_company_ids(company_ids, companies);

	Company *companies_temp = companies->elements;
	for (size_t i = 0; i < companies->num_elements;i++) {

		for (size_t j = 0; j < companies->num_elements;j++) {

			if (companies_temp[j].company_id == company_ids[i]) {

				Vector_PushBack(new_vector, &companies_temp[j]);
				break;

			}			

		}

	}

	Vector_Delete(companies);
	return new_vector;

}

Vector *company_utils_get_random(uint32_t amount)
{

	Vector *companies = company_utils_get_all_active();
	Vector *temp = Vector_Create(sizeof(Company), amount);

	uint32_t real_amount = amount;
	if (real_amount > companies->num_elements)
		real_amount = companies->num_elements;

	Company *companies_temp = companies->elements;
	for (size_t i = 0; i < real_amount;i++)
		Vector_PushBack(temp, &companies_temp[rand()%companies->num_elements]);

	Vector_Delete(companies);

	return temp;

}

Vector *company_utils_get_lowest(uint32_t amount)
{

	Vector *companies = company_utils_get_sorted();
	Vector *temp = Vector_Create(sizeof(Company), amount);

	uint32_t real_amount = amount;
	if (real_amount > companies->num_elements)
		real_amount = companies->num_elements;

	Company *companies_temp = companies->elements;
	for (size_t i = 0; i < real_amount;i++)
		Vector_PushBack(temp, &companies_temp[i]);

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