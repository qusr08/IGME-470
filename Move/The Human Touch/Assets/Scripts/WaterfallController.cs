using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterfallController : MonoBehaviour {
	[SerializeField] private GameObject waterDropletPrefab = null;
	[Space]
	[Tooltip("How many water droplets to spawn per second.")]
	[SerializeField] [Min(0)] public float SpawnRate = 1;
	[Tooltip("The width of the spawn region for the waterfall.")]
	[SerializeField] [Min(0)] public float WaterfallWidth = 1;
	[SerializeField] [Min(0)] public float WaterfallHeight = 0;

	float timer = 0;
	float timePerSpawn = 0;
	int waterDropletsToSpawn = 0;

	private void Start ( ) {
		timePerSpawn = 1 / SpawnRate;
	}

	private void Update ( ) {
		while (timer >= timePerSpawn) {
			waterDropletsToSpawn++;
			timer -= timePerSpawn;
		}

		while (waterDropletsToSpawn > 0) {
			Instantiate(waterDropletPrefab, GetRandomSpawnPosition( ), Quaternion.identity).transform.SetParent(transform);
			waterDropletsToSpawn--;
		}

		timer += Time.deltaTime;
	}

	private Vector3 GetRandomSpawnPosition () {
		return new Vector3(transform.position.x + Random.Range(-WaterfallWidth, WaterfallWidth), transform.position.y + WaterfallHeight, 0);
	}
}
