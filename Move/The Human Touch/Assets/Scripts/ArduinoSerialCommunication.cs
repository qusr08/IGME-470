using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

// https://github.com/GlitchEnzo/NuGetForUnity
// https://www.nuget.org/packages/System.IO.Ports/6.0.0

public class ArduinoSerialCommunication : MonoBehaviour {
	[SerializeField] private WaterfallController waterfallController;
	[SerializeField] private SphereCollider sphereCollider;
	[Space]
	[Tooltip("The force to apply to water droplets when they get near this collider.")]
	[SerializeField] [Min(0)] public float Force;

	SerialPort stream = new SerialPort("COM3", 9600);

	private void OnValidate ( ) {
		waterfallController = FindObjectOfType<WaterfallController>( );
		sphereCollider = GetComponent<SphereCollider>( );
	}

	private void Start ( ) {
		OnValidate( );

		stream.Open( );
		stream.ReadTimeout = 10;
	}

	private void OnTriggerEnter (Collider other) {
		Transform otherTransform = other.transform;
		Rigidbody otherRigidbody = otherTransform.GetComponent<Rigidbody>( );
		WaterfallDropletController otherWaterDropletController = otherTransform.GetComponent<WaterfallDropletController>( );

		// Calculate the angle between the center of this object and the center of the water droplet
		float angleRadians = Mathf.Deg2Rad * Vector3.Angle(-transform.right, (transform.position - otherTransform.position).normalized);
		Vector3 forceVector = new Vector3(Mathf.Cos(angleRadians), Mathf.Sin(angleRadians));

		// Apply a force to the water droplet that will propel it away from this object
		otherRigidbody.AddForce(Force * forceVector);

		// Convert the water droplet to trash
		otherWaterDropletController.ConvertToTrash( );
	}

	private void Update ( ) {
		// If the port is not open, do not try and read from it
		if (!stream.IsOpen) {
			return;
		}

		try {
			// Parse the value to an integer
			// int data;
			// int.TryParse(stream.ReadLine( ), out data);
			int data = int.Parse(stream.ReadLine( ));

			// If the value is -1, then it is an invalid value and should be ignored
			if (data != -1) {
				transform.position = new Vector3(Map(data, 300, 2000, -10, 10), transform.position.y, 0);
			}

			sphereCollider.enabled = (data != -1);
		} catch { }
	}

	private void OnApplicationQuit ( ) {
		stream.Close( );
	}

	// https://forum.unity.com/threads/re-map-a-number-from-one-range-to-another.119437/
	private float Map (float s, float a1, float a2, float b1, float b2) {
		return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
	}
}
