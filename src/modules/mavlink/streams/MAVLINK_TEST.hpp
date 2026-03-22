#ifndef MAVLINK_TEST_HPP
#define MAVLINK_TEST_HPP
#include <uORB/topics/esc_status.h>
#include <uORB/topics/test_mavlink_tx.h>
class MavlinkStreamMavlinktest : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamMavlinktest(mavlink); }

	static constexpr const char *get_name_static() { return "TEST_MAVLINK"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_TEST_MAVLINK; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return test_mavlink_tx_sub.advertised() ? MAVLINK_MSG_ID_TEST_MAVLINK_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES : 0;
	}

private:
	explicit MavlinkStreamMavlinktest(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription test_mavlink_tx_sub{ORB_ID(test_mavlink_tx)};
        uORB::Subscription _esc_status_sub{ORB_ID(esc_status)};
	bool send() override
	{
		esc_status_s escstatus;

		//if (test_mavlink_tx_sub.updated()) {
		if (_esc_status_sub.updated()) {
			mavlink_test_mavlink_t msg{};
			_esc_status_sub.copy(&escstatus);
                        msg.test1=escstatus.esc[7].esc_rpm;
                        msg.test2=2;
                        msg.test3=escstatus.esc[7].esc_voltage;
						// Engine-specific fields are not present in uORB `esc_report_s` on this
						// build. Use safe defaults here to avoid referencing non-existent members.
						msg.oil_pre = 0;
						msg.oil_temp = 0;
						msg.cyl_coolant_temp1 = 0;
						msg.cyl_coolant_temp2 = 0;
						msg.cyl_coolant_temp3 = 0;
						msg.cyl_coolant_temp4 = 0;
						msg.cyl_exh_temp1 = 0;
						msg.cyl_exh_temp2 = 0;
						msg.cyl_exh_temp3 = 0;
						msg.cyl_exh_temp4 = 0;
						msg.volt_14v = 0.0f;
						msg.volt_28v = 0.0f;
			mavlink_msg_test_mavlink_send_struct(_mavlink->get_channel(), &msg);
			return true;
		}

		return false;
	}
};

#endif // MOUNT_ORIENTATION
