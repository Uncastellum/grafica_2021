/*********************************************************************************
Copyright (C) 2014 Adrian Jarabo (ajarabo@unizar.es)
Copyright (C) 2014 Diego Gutierrez (diegog@unizar.es)
All rights reserved.

This is an educational Ray Tracer developed for the course 'Informatica Grafica'
(Computer Graphics) tought at Universidad de Zaragoza (Spain). As such, it does not
intend to be fast or general, but just to provide an educational tool for undergraduate
students.

This software is provided as is, and any express or implied warranties are disclaimed.
In no event shall copyright holders be liable for any damage.
**********************************************************************************/
#include "PhotonMapping.h"
#include "World.h"
#include "Intersection.h"
#include "Ray.h"
#include "BSDF.h"
#include "globals.h"

#include <cmath>

#include <cstdlib>
#include <ctime>
static bool const dummy = (srand(time(NULL)), true);
float rand0_1() {
  return (float) rand() / (RAND_MAX);
}
float rand0_N(int i) {
  return (float) rand() / (RAND_MAX/i);
}

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
   //return is_base_of<Base, T>::value;
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

//*********************************************************************
// Compute the photons by tracing the Ray 'r' from the light source
// through the scene, and by storing the intersections with matter
// in the lists 'xx_photons', storing the diffuse (global) and caustic
// photons respectively. For efficiency, both are computed at the same
// time, since computing them separately would result into a lost of
// several samples marked as caustic or diffuse.
// Same goes with the boolean 'direct', that specifies if direct
// photons (from light to surface) are being stored or not.
// The initial traced photon has energy defined by the tristimulus
// 'p', that accounts for the emitted power of the light source.
// The function will return true when there are more photons (caustic
// or diffuse) to be shot, and false otherwise.
// boolean 'direct' replaced by enum tr_store:
//				NORMAL (like before w/ direct = false)
//				INCLUDE_DIRECT (like before w/ direct = true)
//				DIRECT_ONLY (only stores first photon (light to surface))
//---------------------------------------------------------------------
bool PhotonMapping::trace_ray(const Ray& r, const Vector3 &p,
			   std::list<Photon> &global_photons, std::list<Photon> &caustic_photons, bool direct)
{
	return trace_ray(r,p,global_photons,caustic_photons, direct ? tr_store::NORMAL : tr_store::INCLUDE_DIRECT);
}

bool PhotonMapping::trace_ray(const Ray& r, const Vector3 &p,
			   std::list<Photon> &global_photons, std::list<Photon> &caustic_photons, tr_store ts)
{

	//Check if max number of shots done...
	if( ++m_nb_current_shots > m_max_nb_shots )
	{
		return false;
	}

	// Compute irradiance photon's energy
	Vector3 energy(p);

	Ray photon_ray(r);
	photon_ray.shift();

	bool is_caustic_particle = false;

	//Iterate the path
	while(1)
	{
		// Throw ray and update current_it
		Intersection it;
		world->first_intersection(photon_ray, it);

		if( !it.did_hit() )
			break;

		//Check if has hit a delta material...
		if( it.intersected()->material()->is_delta() )
		{
			// If delta material, then is caustic...
			// Don't store the photon!
			is_caustic_particle = true;
		}
		else if (photon_ray.get_level() > 0 || ts != tr_store::NORMAL)
		{
			//If non-delta material, store the photon!
			if( is_caustic_particle )
			{
				//If caustic particle, store in caustics
				if( caustic_photons.size() < m_nb_caustic_photons )
					caustic_photons.push_back( Photon(it.get_position(), photon_ray.get_direction(), energy ));
			}
			else
			{
				//If non-caustic particle, store in global
				if( global_photons.size() < m_nb_global_photons )
					global_photons.push_back( Photon(it.get_position(), photon_ray.get_direction(), energy ));
			}
			is_caustic_particle = false;
		}

		Real pdf;

		Vector3 surf_albedo = it.intersected()->material()->get_albedo(it);
		Real avg_surf_albedo = surf_albedo.avg();

		Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		while (epsilon2 < 0.)
			epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);

		if (epsilon2 > avg_surf_albedo || photon_ray.get_level() > 20 )
			break;

		if (ts == tr_store::DIRECT_ONLY && !is_caustic_particle && photon_ray.get_level() > 1)
			break;

		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		it.intersected()->material()->get_outgoing_sample_ray(it, photon_ray, pdf );

		// Shade...
		energy = energy*surf_albedo;
		if( !it.intersected()->material()->is_delta() )
			energy *= dot_abs(it.get_normal(), photon_ray.get_direction())/3.14159;

		energy = energy /(pdf*avg_surf_albedo);
	}

	if( caustic_photons.size() == m_nb_caustic_photons &&
		global_photons.size() == m_nb_global_photons )
	{
		m_max_nb_shots = m_nb_current_shots-1;
		return false;
	}

	return true;
}

//*********************************************************************
// TODO: Implement the preprocess step of photon mapping,
// where the photons are traced through the scene. To do it,
// you need to follow these steps for each shoot:
//  1 - Sample a world's light source in the scene to create
//		the initial direct photon from the light source.
//	2 - Trace the photon through the scene storing the inter-
//		sections between the photons and matter. You can use
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, you'll
//		need to build the photon maps, that will be used later
//		for rendering.
//		NOTE: Careful with function
//---------------------------------------------------------------------
void PhotonMapping::preprocess()
{
	std::list<Photon> global_photons, caustic_photons;

	int photon_per_light = m_max_nb_shots / world->nb_lights();

  // for each light
	for (LightSource* ls : world -> light_source_list ) {

		Vector3 point, dir, power;
		Real pdf;
		Ray r;

		do {
      // muestreo
			if (ls->get_no_samples()==1) {

				float phi = 2*M_PI*rand0_1(),
							theta = acos(sqrt(1-rand0_1()));

				dir = Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

				pdf = 1 / (4 * M_PI);
				point = ls -> get_position();
			} else if (false){
        //<----   Other light types go here
      } else {
        //cout << "a";
        break; //unknown
      }

			r = Ray(point, dir);
			power = ls -> get_intensities() / (photon_per_light * pdf);

      //through all scene
		} while (trace_ray(r, power, global_photons, caustic_photons, tr_store::NORMAL));

	}

	for (Photon ph : global_photons) {
		std::vector<Real> pos(3);
		pos[0] = ph.position.data[0];
		pos[1] = ph.position.data[1];
		pos[2] = ph.position.data[2];
		m_global_map.store(pos, ph);
	}
	if (!global_photons.empty()) m_global_map.balance();

	for (Photon ph : caustic_photons) {
		std::vector<Real> pos(3);
		pos[0] = ph.position.data[0];
		pos[1] = ph.position.data[1];
		pos[2] = ph.position.data[2];
		m_caustics_map.store(pos, ph);
	}
	if (!caustic_photons.empty()) m_caustics_map.balance();
}

//*********************************************************************
// TODO: Implement the function that computes the rendering equation
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. Note that you will need to handle
// both direct and global illumination, together with recursive the
// recursive evaluation of delta materials. For an optimal implemen-
// tation you should be able to do it iteratively.
// In principle, the class is prepared to perform radiance estimation
// using k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Vector3 PhotonMapping::shade(Intersection &it0) const
{
  Vector3 L_l(0), L_s(0), L_c(0), L_d(0);
  Intersection it(it0);
  Vector3 kd = it.intersected()->material()->get_albedo(it);

  //Compute direct illumination
  for (LightSource* light : world->light_source_list) {
    if(instanceof<PointLightSource>(light)) {

      Vector3 wi = light->get_incoming_direction(it.get_position()).normalize() * -1;
      Vector3 wo = it.get_ray().get_direction();
			Vector3 wr = wi.reflect(it.get_normal());
			Vector3 brdf(0);
      Real alpha = it.intersected()->material()->get_specular(it);
      if (alpha == 0. || alpha == INFINITY)
      {
          //Lambertian
					brdf = kd / M_PI;
      }else{
          //Phong
					brdf = kd * (alpha + 2) * powf(wo.dot_abs(wr), alpha) / (2 * M_PI);
      }
      L_l = L_l + ( light->get_incoming_light(it.get_position()) *
            brdf * it.get_normal().dot_abs(wi) );

    } else {;} //Other light type
  }

  //Compute DIFF Reflections && Caustics
  Real max_distance_g, max_distance_c;
  std::vector<Real> pos(3);
  std::vector<const KDTree<Photon, 3>::Node *> nodes_global;
  std::vector<const KDTree<Photon, 3>::Node *> nodes_caustic;
  pos[0] = it.get_position().data[0];
  pos[1] = it.get_position().data[1];
  pos[2] = it.get_position().data[2];

  m_global_map.find(pos, m_nb_photons, nodes_global, max_distance_g);
  m_caustics_map.find(pos, m_nb_photons, nodes_caustic, max_distance_c);

  Real c_area = M_PI * max_distance_c * max_distance_c,
       g_area = M_PI * max_distance_g * max_distance_g;

  for (const KDTree<Photon, 3>::Node* p : nodes_global) {
    Vector3 wo = it.get_ray().get_direction(), wi = p->data().direction * -1;
		Real alpha = it.intersected()->material()->get_specular(it);
		Vector3 brdf;
		Vector3 wr = wi.reflect(it.get_normal());
		if (alpha == 0. || alpha == INFINITY){
			//Lambertiano
			brdf = kd / M_PI;
		}
		else{
      //Phong
			Real p_dot = std::max(dot(wo, wr), 0.f);
			brdf = kd * (alpha + 2) * powf(p_dot, alpha) / (2 * M_PI);
		}
		L_d = L_d + p->data().flux * brdf;
  }
  L_d = L_d/g_area;

  for (const KDTree<Photon, 3>::Node* p : nodes_caustic) {
    Vector3 wo = it.get_ray().get_direction(), wi = p->data().direction * -1;
		Real alpha = it.intersected()->material()->get_specular(it);
		Vector3 brdf;
		Vector3 wr = wi.reflect(it.get_normal());
		if (alpha == 0. || alpha == INFINITY){
			//Lambertiano
			brdf = kd / M_PI;
		}
		else{
      //Phong
			Real p_dot = std::max(dot(wo, wr), 0.f);
			brdf = kd * (alpha + 2) * powf(p_dot, alpha) / (2 * M_PI);
		}
		L_c = L_c + p->data().flux * brdf;
  }
  L_c = L_c/c_area;

  //Compute specular reflection/refraction
  Vector3 W(1);
  if(it.intersected()->material()->is_delta()) {
    for (size_t i = 0; i < MAX_NB_SPECULAR_BOUNCES; i++) {
      Ray r;
  		float pdf;
      it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf);
      W = W * it.intersected()->material()->get_albedo(it) / pdf;
      r.shift();
  		world->first_intersection(r, it);
    }
    L_s = L_s * W;
  }
  return L_l + L_s + L_c + L_d;
}
