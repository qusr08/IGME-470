using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterfallDropletController : MonoBehaviour {
	[SerializeField] private MeshRenderer meshRenderer;
	[Space]
	[SerializeField] private List<GameObject> trashModels;

	private GameObject randomTrashModel;

	private void OnValidate ( ) {
		meshRenderer = GetComponent<MeshRenderer>( );
	}

	private void Start ( ) {
		OnValidate( );

		transform.rotation = Quaternion.Euler(Random.Range(0, 360), Random.Range(0, 360), Random.Range(0, 360));

		// Spawn a piece of trash that follows the water droplet
		randomTrashModel = Instantiate(trashModels[Random.Range(0, trashModels.Count)], transform.position, Quaternion.identity);
		randomTrashModel.transform.SetParent(transform);
		randomTrashModel.SetActive(false);
	}

	private void Update ( ) {
		if (transform.position.y <= 0) {
			Destroy(gameObject);
		}
	}

	public void ConvertToTrash ( ) {
		// Set this game object to the trash layer so it interacts with the other water droplets
		gameObject.layer = LayerMask.NameToLayer("Trash");

		// Disable the water droplet mesh renderer and enable the trash model
		meshRenderer.enabled = false;
		randomTrashModel.SetActive(true);
	}
}
